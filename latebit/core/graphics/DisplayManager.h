#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "Colors.h"
#include "Font.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/utils/Manager.h"

using namespace std;

#define DM lb::DisplayManager::getInstance()

namespace lb {

namespace TextAlignment {
enum TextAlignment {
  LEFT,
  CENTER,
  RIGHT,
};
}

namespace TextSize {
enum TextSize {
  NORMAL = 1,
  LARGE,
  XLARGE,
  XXLARGE,
};
}

// A 2D Vector representing a position in the world in cells
using Position = Vector;

const int WINDOW_HORIZONTAL_CELLS = 240;
const int WINDOW_VERTICAL_CELLS = 160;
const Color::Color WINDOW_BACKGROUND_COLOR_DEFAULT = Color::BLACK;

const string DUMMY_VIDEODRIVER = "dummy";

// Converts cell coodinates to pixel coordinates
auto cellsToPixels(Position cells) -> Vector;
// Converts pixel coodinates to cell coordinates
auto pixelsToCells(Vector pixels) -> Position;

class DisplayManager : public Manager {
 private:
  DisplayManager();
  // The window has an initial given size in pixels and cells. All we draw
  // are characters and all the coordinates we use are in cells
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  // Window width in cells
  int widthInCells = WINDOW_HORIZONTAL_CELLS;
  // Window height in cells
  int heightInCells = WINDOW_VERTICAL_CELLS;
  // Background color of the window
  Color::Color backgroundColor = WINDOW_BACKGROUND_COLOR_DEFAULT;

  // Returns appropriate renderer flags for the current driver
  [[nodiscard]] auto getRendererFlags() const -> int;

 public:
  DisplayManager(DisplayManager const &) = delete;
  void operator=(DisplayManager const &) = delete;
  static auto getInstance() -> DisplayManager &;

  // Sets up the window and font
  auto startUp() -> int override;
  void shutDown() override;

  // Draws a frame in the given position, scaling it by the given factor
  [[nodiscard]] auto drawFrame(Position position, const Keyframe *frame,
                               int scaling = 1) const -> int;

  // Draws a rectangle outline at the given world position (top left cell)
  [[nodiscard]] auto drawRectangle(Position position, int width, int height,
                                   Color::Color borderColor) const -> int;

  // Draws a rectangle at the given world position (top left cell)
  [[nodiscard]] auto drawRectangle(Position position, int width, int height,
                                   Color::Color borderColor,
                                   Color::Color fillColor) const -> int;

  // Draws a string to the window at the given world position
  [[nodiscard]] auto drawString(Position postion, string string,
                                TextAlignment::TextAlignment alignment,
                                Color::Color color,
                                TextSize::TextSize size = TextSize::NORMAL,
                                Font font = DEFAULT_FONT) const -> int;

  // Returns the bounding box of a given string. Dimensions are in cells
  [[nodiscard]] auto measureString(string string,
                                   TextSize::TextSize size = TextSize::NORMAL,
                                   Font font = DEFAULT_FONT) const -> Box;

  // Change the background color of the window
  void setBackground(Color::Color color);

  // Return the amount of horizontal cells
  [[nodiscard]] auto getHorizontalCells() const -> int;
  // Return the amount of vertical cells
  [[nodiscard]] auto getVerticalCells() const -> int;

  // Swap the buffers for drawing.
  // This is the result of Double Buffering: first we draw to a hidden buffer
  // and then we display it to the screen
  auto swapBuffers() -> int;
};
}  // namespace lb
