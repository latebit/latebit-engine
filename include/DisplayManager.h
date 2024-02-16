#pragma once

#include <SFML/Graphics.hpp>

#include "Colors.h"
#include "Manager.h"
#include "Vector.h"

using namespace std;

#define DM df::DisplayManager::getInstance()

namespace df {

enum Alignment {
  ALIGN_LEFT,
  ALIGN_CENTER,
  ALIGN_RIGHT,
};

// A 2D Vector representing a position in the world in cells
using Position = Vector;

const int WINDOW_HORIZONTAL_CELLS = 240;
const int WINDOW_VERTICAL_CELLS = 160;
const float CELL_SIZE = 3.0f;
const int WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
const Color WINDOW_BACKGROUND_COLOR_DEFAULT = BLACK;
const string WINDOW_TITLE_DEFAULT = "New Game";
const string FONT_FILE_DEFAULT = "df-font.ttf";
const uint FONT_SIZE_DEFAULT = CELL_SIZE * 6;

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
  sf::Font font = sf::Font();
  // The window has an initial given size in pixels and cells. All we draw
  // are characters and all the coordinates we use are in cells
  sf::RenderWindow *window = nullptr;

  // Window width in cells
  int widthInCells = WINDOW_HORIZONTAL_CELLS;
  // Window height in cells
  int heightInCells = WINDOW_VERTICAL_CELLS;
  // Background color of the window
  Color backgroundColor = WINDOW_BACKGROUND_COLOR_DEFAULT;

  // Returns an sf::Text object with the default font and the given parameters
  // It does not draw the text to the window. It's meant to be used to preview
  // the text before drawing it
  auto makeText(Position position, string string, Alignment alignment,
                Color color) const -> sf::Text;

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
  auto drawRectangle(Position position, int width, int height,
                     Color borderColor) const -> int;

  // Draws a rectangle at the given world position (top left cell)
  auto drawRectangle(Position position, int width, int height,
                     Color borderColor, Color fillColor) const -> int;

  // Draws a string to the window at the given world position
  auto drawString(Position postion, string string, Alignment alignment,
                  Color color) const -> int;

  // Returns the size of the string in cells
  auto measureString(string string) const -> Box;

  // Change the background color of the window
  void setBackground(Color color);

  // Return the amount of horizontal cells
  auto getHorizontalCells() const -> int;
  // Return the amount of vertical cells
  auto getVerticalCells() const -> int;

  // Returns the size of a character in cells
  [[nodiscard]] auto getFontSize() const -> int;

  // Swap the buffers for drawing.
  // This is the result of Double Buffering: first we draw to a hidden buffer
  // and then we display it to the screen
  auto swapBuffers() -> int;

  // Returns a pointer to the window
  [[nodiscard]] auto getWindow() const -> sf::RenderWindow *;
};
}  // namespace df
