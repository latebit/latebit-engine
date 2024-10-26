#include "DisplayManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <sys/types.h>

#include <cstdint>

#include "Colors.h"
#include "Font.h"
#include "Keyframe.h"
#include "latebit/core/configuration/Configuration.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/utils/Manager.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Math.h"

namespace lb {

const int CELL_SIZE = 3;

auto cellsToPixels(Position spaces) -> Vector {
  return {spaces.getX() * CELL_SIZE, spaces.getY() * CELL_SIZE};
}

auto pixelsToCells(Vector pixels) -> Position {
  return {pixels.getX() / CELL_SIZE, pixels.getY() / CELL_SIZE};
}

DisplayManager::DisplayManager() : Manager("DisplayManager") {
  Log.debug("DisplayManager::DisplayManager(): Created DisplayManager");
}

auto DisplayManager::startUp() -> int {
  if (this->isStarted()) return 0;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0) {
    Log.error("DisplayManager::startUp(): Cannot initiate SDL. %s",
              SDL_GetError());
    return -1;
  }

  auto widthInPixels = WINDOW_WIDTH * CELL_SIZE;
  auto heightInPixels = WINDOW_HEIGHT * CELL_SIZE;
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
  Log.info("DisplayManager::shutDown(): Shut down successfully");
  return Manager::shutDown();
}

auto DisplayManager::getRendererFlags() -> int {
  return string(SDL_GetCurrentVideoDriver()) == DUMMY_VIDEODRIVER
           ? 0
           : SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

auto DisplayManager::drawKeyframe(Position position, const Keyframe* frame,
                                  uint8_t width, uint8_t height,
                                  uint8_t scaling) -> int {
  if (!this->isStarted()) {
    Log.error("DisplayManager::drawFrame(): Display Manager is not started");
    return -1;
  }

  if (frame->size() != width * height) {
    Log.error(
      "DisplayManager::drawFrame(): Invalid frame size. Expected %d, got %d",
      width * height, frame->size());
    return -1;
  }

  if (frame->size() == 0) {
    return 0;
  }

  scaling = clamp(scaling, 1, 10);
  auto viewPosition = WM.getView().worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);
  auto cellSize = CELL_SIZE * scaling;

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
    0, width * cellSize, height * cellSize, 32, SDL_PIXELFORMAT_RGBA32);

  if (surface == nullptr) {
    Log.error("DisplayManager::drawFrame(): Cannot create surface. %s",
              SDL_GetError());
    return -1;
  }

  for (uint8_t i = 0; i < height; i++) {
    for (uint8_t j = 0; j < width; j++) {
      auto index = i * width + j;
      auto color = toSDLColor(frame->at(index));

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
                        width * cellSize, height * cellSize};

  SDL_RenderCopy(this->renderer, texture, nullptr, &rectangle);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

  return 0;
}

auto DisplayManager::fillRectangle(Position position, uint8_t width,
                                   uint8_t height, Color::Color color) -> int {
  if (this->window == nullptr) return -1;

  if (color != Color::UNDEFINED_COLOR) {
    auto viewPosition = WM.getView().worldToView(position);
    auto pixelPosition = cellsToPixels(viewPosition);

    SDL_Rect rectangle = {(int)pixelPosition.getX(), (int)pixelPosition.getY(),
                          width * CELL_SIZE, height * CELL_SIZE};

    auto fill = toSDLColor(color);
    SDL_SetRenderDrawColor(this->renderer, fill.r, fill.g, fill.b, fill.a);
    SDL_RenderFillRect(this->renderer, &rectangle);
  }

  return 0;
}

auto DisplayManager::strokeRectangle(Position position, uint8_t width,
                                     uint8_t height,
                                     Color::Color color) -> int {
  if (this->window == nullptr) return -1;

  if (color != Color::UNDEFINED_COLOR) {
    auto viewPosition = WM.getView().worldToView(position);
    auto pixelPosition = cellsToPixels(viewPosition);

    SDL_Rect rectangle = {(int)pixelPosition.getX(), (int)pixelPosition.getY(),
                          width * CELL_SIZE, height * CELL_SIZE};

    auto fill = toSDLColor(color);
    SDL_SetRenderDrawColor(this->renderer, fill.r, fill.g, fill.b, fill.a);
    SDL_RenderDrawRect(this->renderer, &rectangle);
  }

  return 0;
}

auto DisplayManager::drawString(Position position, string string,
                                TextAlignment::TextAlignment alignment,
                                Color::Color color, TextSize::TextSize size,
                                Font font) -> int {
  if (this->window == nullptr) return -1;

  Position viewPosition = WM.getView().worldToView(position);
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

    if (this->drawKeyframe(position, &content, gWidth, gHeight, size) != 0) {
      return -1;
    }
  }
  return 0;
}

auto DisplayManager::measureString(string string, TextSize::TextSize size,
                                   Font font) -> Box {
  int len = string.size();
  int gWidth = font.getGlyphWidth();
  int gHeight = font.getGlyphHeight();
  float lineWidth = len * gWidth * size + len * font.getHorizontalSpacing();
  float lineHeight = gHeight * size;

  return {lineWidth, lineHeight};
}

void DisplayManager::setBackground(Color::Color color) {
  this->backgroundColor = color;
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

auto DisplayManager::getInstance() -> DisplayManager& {
  static DisplayManager instance;
  return instance;
}

}  // namespace lb
