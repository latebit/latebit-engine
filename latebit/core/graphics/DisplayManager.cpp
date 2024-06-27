#include "DisplayManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "Colors.h"
#include "Font.h"
#include "Keyframe.h"
#include "latebit/core/configuration/Configuration.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/WorldManager.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Math.h"

namespace lb {

const int CELL_SIZE = 3;

DisplayManager::DisplayManager() : Manager("DisplayManager") {
  Log.debug("DisplayManager::DisplayManager(): Created DisplayManager");
}

auto DisplayManager::getInstance() -> DisplayManager& {
  static DisplayManager instance;
  return instance;
}

auto DisplayManager::startUp() -> int {
  if (this->window != nullptr) {
    return 0;
  }

  auto widthInPixels = this->widthInCells * CELL_SIZE;
  auto heightInPixels = this->heightInCells * CELL_SIZE;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0) {
    Log.error("DisplayManager::startUp(): Cannot initiate SDL. %s",
              SDL_GetError());
    return -1;
  }

  this->window = SDL_CreateWindow(
    Configuration::getInitialWindowTitle().c_str(), SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, widthInPixels, heightInPixels, SDL_WINDOW_SHOWN);

  if (this->window == nullptr) {
    Log.error("DisplayManager::startUp(): Cannot create window. %s",
              SDL_GetError());
    return -1;
  }

  this->renderer =
    SDL_CreateRenderer(this->window, -1, this->getRendererFlags());
  if (this->renderer == nullptr) {
    Log.error("DisplayManager::startUp(): Cannot create renderer. %s",
              SDL_GetError());
    this->shutDown();
    return -1;
  }

  Log.info("DisplayManager::startUp(): Started successfully");
  return Manager::startUp();
}

void DisplayManager::shutDown() {
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);

  this->window = nullptr;
  this->renderer = nullptr;
  SDL_Quit();
  Manager::shutDown();
  Log.info("DisplayManager::shutDown(): Shut down successfully");
}

auto DisplayManager::getRendererFlags() const -> int {
  return string(SDL_GetCurrentVideoDriver()) == DUMMY_VIDEODRIVER
           ? 0
           : SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

auto DisplayManager::drawFrame(Position position, const Keyframe* frame,
                               int scaling) const -> int {
  if (this->window == nullptr) {
    Log.error("DisplayManager::drawFrame(): Window is null");
    return -1;
  }

  scaling = clamp(scaling, 1, 10);
  auto viewPosition = WorldManager::worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);
  auto content = frame->getContent();
  auto cellSize = CELL_SIZE * scaling;

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
    0, frame->getWidth() * cellSize, frame->getHeight() * cellSize, 32,
    SDL_PIXELFORMAT_RGBA32);

  if (surface == nullptr) {
    Log.error("DisplayManager::drawFrame(): Cannot create surface. %s",
              SDL_GetError());
    return -1;
  }

  for (uint8_t i = 0; i < frame->getHeight(); i++) {
    for (uint8_t j = 0; j < frame->getWidth(); j++) {
      auto index = i * frame->getWidth() + j;
      auto color = toSDLColor(content[index]);

      // Iterate over each pixel cellSize times in both width and height
      for (int k = 0; k < cellSize; k++) {
        for (int l = 0; l < cellSize; l++) {
          // Calculate the position in the surface to draw the pixel
          int x = j * cellSize + l;
          int y = i * cellSize + k;

          // Calculate the index in the surface's pixel buffer
          int surfaceIndex = y * surface->pitch + x * sizeof(Uint32);

          // Map the color to an SDL-compatible format
          Uint32 pixelColor =
            SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

          // Write the color to the surface's pixel buffer
          *((Uint32*)((Uint8*)surface->pixels + surfaceIndex)) = pixelColor;
        }
      }
    }
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);

  if (texture == nullptr) {
    Log.error("DisplayManager::drawFrame(): Cannot create texture. %s",
              SDL_GetError());
    return -1;
  }

  SDL_Rect rectangle = {(int)pixelPosition.getX(), (int)pixelPosition.getY(),
                        (int)frame->getWidth() * cellSize,
                        (int)frame->getHeight() * cellSize};

  SDL_RenderCopy(this->renderer, texture, nullptr, &rectangle);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

  return 0;
}

auto DisplayManager::drawRectangle(Position position, int width, int height,
                                   Color::Color borderColor,
                                   Color::Color fillColor) const -> int {
  if (this->window == nullptr) return -1;

  auto viewPosition = WorldManager::worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);

  SDL_Rect rectangle = {(int)pixelPosition.getX(), (int)pixelPosition.getY(),
                        width * CELL_SIZE, height * CELL_SIZE};

  if (fillColor != Color::UNDEFINED_COLOR) {
    auto fill = toSDLColor(fillColor);
    SDL_SetRenderDrawColor(this->renderer, fill.r, fill.g, fill.b, fill.a);
    SDL_RenderFillRect(this->renderer, &rectangle);
  }

  auto border = toSDLColor(borderColor);
  SDL_SetRenderDrawColor(this->renderer, border.r, border.g, border.b,
                         border.a);
  SDL_RenderDrawRect(this->renderer, &rectangle);

  return 0;
}

auto DisplayManager::drawRectangle(Position position, int width, int height,
                                   Color::Color borderColor) const -> int {
  return drawRectangle(position, width, height, borderColor,
                       Color::UNDEFINED_COLOR);
}

auto DisplayManager::drawString(Position position, string string,
                                TextAlignment::TextAlignment alignment,
                                Color::Color color, TextSize::TextSize size,
                                Font font) const -> int {
  if (this->window == nullptr) return -1;

  Position viewPosition = WorldManager::worldToView(position);
  int len = string.size();
  int gWidth = font.getGlyphWidth();
  int gHeight = font.getGlyphHeight();
  int lineWidth = len * gWidth * size + len * font.getHorizontalSpacing();

  // Draw each character in the string in a separate frame
  // This is far simpler then trying to draw the string as a whole with only
  // one frame but might also be less efficient
  for (int i = 0; i < len; i++) {
    auto glyph = font.getGlyph(string[i]);

    // Populate the content vector with the color of the glyph
    auto content = vector<Color::Color>();
    for (int y = gHeight - 1; y >= 0; y--) {
      for (int x = gWidth - 1; x >= 0; x--) {
        content.push_back(glyph[y * gWidth + x] ? color
                                                : Color::UNDEFINED_COLOR);
      }
    }

    auto frame = Keyframe(gWidth, gHeight, content);
    auto position =
      viewPosition +
      Vector(gWidth * i * size + font.getHorizontalSpacing() * i, 0);

    switch (alignment) {
      case TextAlignment::CENTER:
        position.setX(position.getX() - lineWidth / 2);
        break;
      case TextAlignment::RIGHT:
        position.setX(position.getX() - lineWidth);
        break;
      case TextAlignment::LEFT:
        break;
    }

    if (drawFrame(position, &frame, size) != 0) {
      return -1;
    }
  }
  return 0;
}

auto DisplayManager::measureString(string string, TextSize::TextSize size,
                                   Font font) const -> Box {
  int len = string.size();
  int gWidth = font.getGlyphWidth();
  int gHeight = font.getGlyphHeight();
  float lineWidth = len * gWidth * size + len * font.getHorizontalSpacing();
  float lineHeight = gHeight * size;

  auto cellBounds = pixelsToCells({lineWidth, lineHeight});

  return {cellBounds.getX(), cellBounds.getY()};
}

void DisplayManager::setBackground(Color::Color color) {
  this->backgroundColor = color;
}

auto DisplayManager::getHorizontalCells() const -> int {
  return this->widthInCells;
}

auto DisplayManager::getVerticalCells() const -> int {
  return this->heightInCells;
}

auto DisplayManager::swapBuffers() -> int {
  if (this->window == nullptr) {
    Log.error("DisplayManager::swapBuffers(): Window is null");
    return -1;
  }

  // displays current buffer
  SDL_RenderPresent(this->renderer);

  // clears second buffer
  auto c = toSDLColor(this->backgroundColor);
  SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, c.a);
  SDL_RenderClear(this->renderer);

  return 0;
}

auto cellsToPixels(Position spaces) -> Vector {
  return {spaces.getX() * CELL_SIZE, spaces.getY() * CELL_SIZE};
}

auto pixelsToCells(Vector pixels) -> Position {
  return {pixels.getX() / CELL_SIZE, pixels.getY() / CELL_SIZE};
}

}  // namespace lb
