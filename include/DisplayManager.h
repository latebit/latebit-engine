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

const int WINDOW_HORIZONTAL_PIXELS_DEFAULT = 1024;
const int WINDOW_VERTICAL_PIXELS_DEFAULT = 768;
const int WINDOW_HORIZONTAL_CHARS_DEFAULT = 80;
const int WINDOW_VERTICAL_CHARS_DEFAULT = 24;
const int WINDOW_STYLE_DEFAULT = sf::Style::Titlebar | sf::Style::Close;
const Color WINDOW_BACKGROUND_COLOR_DEFAULT = BLACK;
const string WINDOW_TITLE_DEFAULT = "Dragonfly";
const string FONT_FILE_DEFAULT = "df-font.ttf";

// Returns height of a single character
auto charHeight() -> float;
// Returns width of a single character
auto charWidth() -> float;

// Converts cell coodinates to pixel coordinates
auto cellsToPixels(Vector cells) -> Vector;
// Converts pixel coodinates to cell coordinates
auto pixelsToCells(Vector pixels) -> Vector;

class DisplayManager : public Manager {
 private:
  DisplayManager();
  // Font used to draw text
  sf::Font font = sf::Font();
  // The window has an initial given size in pixels and cells. All we draw
  // are characters and all the coordinates we use are in cells
  sf::RenderWindow *window = nullptr;
  // Window width in pixels
  int widthInPixels = WINDOW_HORIZONTAL_PIXELS_DEFAULT;
  // Window height in pixels
  int heightInPixels = WINDOW_VERTICAL_PIXELS_DEFAULT;
  // Window width in cells
  int widthInCells = WINDOW_HORIZONTAL_CHARS_DEFAULT;
  // Window height in cells
  int heightInCells = WINDOW_VERTICAL_CHARS_DEFAULT;
  // Background color of the window
  Color backgroundColor = WINDOW_BACKGROUND_COLOR_DEFAULT;

 public:
  DisplayManager(DisplayManager const &) = delete;
  void operator=(DisplayManager const &) = delete;
  static auto getInstance() -> DisplayManager &;

  // Sets up the window and font
  auto startUp() -> int override;
  void shutDown() override;

  // Draws a single character to the window at the given position
  auto drawCh(Vector position, char c, Color color) const -> int;
  // Draws a single character to the window at the given position with
  // the given background color
  auto drawCh(Vector position, char c, Color foreground, Color background) const
    -> int;

  // Draws a string to the window at the given world position
  auto drawString(Vector postion, string s, Alignment a, Color color) const
    -> int;
  // Draws a string to the window at the given world position with the given
  // background color
  auto drawString(Vector position, string s, Alignment a, Color foreground,
                  Color background) const -> int;

  // Change the background color of the window
  void setBackground(Color color);

  // Return the amount of horizontal cells
  auto getHorizontalCells() const -> int;
  // Return the amount of vertical cells
  auto getVerticalCells() const -> int;
  // Return the window width in pixels
  auto getHorizontalPixels() const -> int;
  // Return the window height in pixels
  auto getVerticalPixels() const -> int;

  // Swap the buffers for drawing.
  // This is the result of Double Buffering: first we draw to a hidden buffer
  // and then we display it to the screen
  auto swapBuffers() -> int;

  // Returns a pointer to the window
  [[nodiscard]] auto getWindow() const -> sf::RenderWindow *;
};
}  // namespace df
