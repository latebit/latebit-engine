#include "DisplayManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "Colors.h"
#include "Configuration.h"
#include "Font.h"
#include "Frame.h"
#include "Logger.h"
#include "Vector.h"
#include "utils.h"

namespace lb {

const int CELL_SIZE = 3;

auto getLineWidth(string line) -> int {
  return line.size() *
         (DEFAULT_FONT.glyphWidth + DEFAULT_FONT.horizontalSpacing);
}

auto getLineHeight(string line) -> int { return DEFAULT_FONT.glyphHeight; }

DisplayManager::DisplayManager() {
  setType("DisplayManager");
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

  if (TTF_Init() != 0) {
    Log.error("DisplayManager::startUp(): Cannot initiate TTF. %s",
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

auto DisplayManager::drawFrame(Position position, const Frame* frame) const
  -> int {
  if (this->window == nullptr) {
    Log.error("DisplayManager::drawFrame(): Window is null");
    return -1;
  }

  auto viewPosition = worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);
  auto content = frame->getContent();

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
    0, frame->getWidth() * CELL_SIZE, frame->getHeight() * CELL_SIZE, 32,
    SDL_PIXELFORMAT_RGBA32);

  if (surface == nullptr) {
    Log.error("DisplayManager::drawFrame(): Cannot create surface. %s",
              SDL_GetError());
    return -1;
  }

  for (int i = 0; i < frame->getHeight(); i++) {
    for (int j = 0; j < frame->getWidth(); j++) {
      auto index = i * frame->getWidth() + j;
      auto color = toSDLColor(content[index]);

      // Iterate over each pixel CELL_SIZE times in both width and height
      for (int k = 0; k < CELL_SIZE; k++) {
        for (int l = 0; l < CELL_SIZE; l++) {
          // Calculate the position in the surface to draw the pixel
          int x = j * CELL_SIZE + l;
          int y = i * CELL_SIZE + k;

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
                        frame->getWidth() * CELL_SIZE,
                        frame->getHeight() * CELL_SIZE};

  SDL_RenderCopy(this->renderer, texture, nullptr, &rectangle);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

  return 0;
}

auto DisplayManager::drawRectangle(Position position, int width, int height,
                                   Color borderColor, Color fillColor) const
  -> int {
  if (this->window == nullptr) return -1;

  auto viewPosition = worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);

  SDL_Rect rectangle = {(int)pixelPosition.getX(), (int)pixelPosition.getY(),
                        width * CELL_SIZE, height * CELL_SIZE};

  if (fillColor != UNDEFINED_COLOR) {
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
                                   Color borderColor) const -> int {
  return drawRectangle(position, width, height, borderColor, UNDEFINED_COLOR);
}

auto DisplayManager::drawString(Position position, string string,
                                Alignment alignment, Color color,
                                Font font) const -> int {
  if (this->window == nullptr) return -1;

  auto viewPosition = worldToView(position);

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
    0, font.glyphWidth * CELL_SIZE * string.size(),
    font.glyphHeight * CELL_SIZE, 16, SDL_PIXELFORMAT_RGBA32);

  if (surface == nullptr) {
    Log.error("DisplayManager::drawFrame(): Cannot create surface. %s",
              SDL_GetError());
    return -1;
  }

  auto lineWidth = getLineWidth(string);

  for (int i = 0; i < string.size(); i++) {
    // Find ascii code for the char
    auto code = static_cast<int>(string[i]);

    // Find the glyph corresponding to the code
    auto glyph = font.glyphs.at(code - 32);

    // Populate the content vector with the color of the glyph
    auto content = vector<Color>();
    for (int y = font.glyphHeight - 1; y >= 0; y--) {
      for (int x = font.glyphWidth - 1; x >= 0; x--) {
        content.push_back(glyph[y * font.glyphWidth + x] ? color
                                                         : UNDEFINED_COLOR);
      }
    }

    auto frame = Frame(font.glyphWidth, font.glyphHeight, content);
    auto position =
      viewPosition + Vector((font.glyphWidth + font.horizontalSpacing) * i, 0);

    switch (alignment) {
      case ALIGN_CENTER:
        position.setX(position.getX() - lineWidth / 2);
        break;
      case ALIGN_RIGHT:
        position.setX(position.getX() - lineWidth);
        break;
      case ALIGN_LEFT:
        break;
    }

    drawFrame(position, &frame);
  }

  return 0;
}

auto DisplayManager::measureString(string string, Font font) const -> Box {
  int h = font.glyphHeight, w = getLineWidth(string);

  auto cellBounds =
    pixelsToCells({static_cast<float>(w), static_cast<float>(h)});

  return {cellBounds.getX(), cellBounds.getY()};
}

void DisplayManager::setBackground(Color color) {
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
