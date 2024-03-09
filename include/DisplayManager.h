#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "Colors.h"
#include "Manager.h"
#include "Vector.h"

using namespace std;

#define DM lb::DisplayManager::getInstance()

namespace lb {

enum Alignment {
  ALIGN_LEFT,
  ALIGN_CENTER,
  ALIGN_RIGHT,
};

// A 2D Vector representing a position in the world in cells
using Position = Vector;

const int WINDOW_HORIZONTAL_CELLS = 240;
const int WINDOW_VERTICAL_CELLS = 160;
const int CELL_SIZE = 3;
const Color WINDOW_BACKGROUND_COLOR_DEFAULT = BLACK;
const string WINDOW_TITLE_DEFAULT = "Latebits";
const string FONT_FILE_DEFAULT = "font.ttf";
const uint FONT_SIZE_DEFAULT = CELL_SIZE * 8;

const string DUMMY_VIDEODRIVER = "dummy";

// Returns height of a single character
auto charHeight() -> float;
// Returns width of a single character
auto cellSize() -> float;

// Converts cell coodinates to pixel coordinates
auto cellsToPixels(Position cells) -> Vector;
// Converts pixel coodinates to cell coordinates
auto pixelsToCells(Vector pixels) -> Position;

class DisplayManager : public Manager {
 private:
  DisplayManager();
  // Font used to draw text
  TTF_Font *font = nullptr;
  // The window has an initial given size in pixels and cells. All we draw
  // are characters and all the coordinates we use are in cells
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  // Window width in cells
  int widthInCells = WINDOW_HORIZONTAL_CELLS;
  // Window height in cells
  int heightInCells = WINDOW_VERTICAL_CELLS;
  // Background color of the window
  Color backgroundColor = WINDOW_BACKGROUND_COLOR_DEFAULT;

  // Returns appropriate renderer flags for the current driver
  [[nodiscard]] auto getRendererFlags() const -> uint;

 public:
  DisplayManager(DisplayManager const &) = delete;
  void operator=(DisplayManager const &) = delete;
  static auto getInstance() -> DisplayManager &;

  // Sets up the window and font
  auto startUp() -> int override;
  void shutDown() override;

  // Draws a frame in the given position
  auto drawFrame(Position position, const Frame *frame) const -> int;

  // Draws a rectangle outline at the given world position (top left cell)
  [[nodiscard]] auto drawRectangle(Position position, int width, int height,
                                   Color borderColor) const -> int;

  // Draws a rectangle at the given world position (top left cell)
  [[nodiscard]] auto drawRectangle(Position position, int width, int height,
                                   Color borderColor, Color fillColor) const
    -> int;

  // Draws a string to the window at the given world position
  [[nodiscard]] auto drawString(Position postion, string string,
                                Alignment alignment, Color color) const -> int;

  // Returns the bounding box of a given string. Dimensions are in cells
  [[nodiscard]] auto measureString(string string) const -> Box;

  // Change the background color of the window
  void setBackground(Color color);

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
