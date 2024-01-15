#pragma once

#include <SFML/Graphics.hpp>

#include "Colors.h"
#include "Manager.h"
#include "Vector.h"

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
const std::string WINDOW_TITLE_DEFAULT = "Dragonfly";
const std::string FONT_FILE_DEFAULT = "df-font.ttf";

auto charHeight() -> float;
auto charWidth() -> float;

auto spacesToPixels(Vector spaces) -> Vector;
auto pixelsToSpaces(Vector pixels) -> Vector;

class DisplayManager : public Manager {
 private:
  DisplayManager();
  DisplayManager(DisplayManager const &);
  void operator=(DisplayManager const &);
  sf::Font font;
  sf::RenderWindow *window;
  int window_horizontal_pixels;
  int window_vertical_pixels;
  int window_horizontal_cells;
  int window_vertical_cells;
  Color background_color;

 public:
  static auto getInstance() -> DisplayManager &;

  auto startUp() -> int override;

  void shutDown() override;

  auto drawCh(Vector world_pos, char ch, Color fg) const -> int;
  auto drawCh(Vector world_pos, char ch, Color fg, Color bg) const -> int;

  auto drawString(Vector world_pos, std::string s, Alignment a, Color fg) const
    -> int;
  auto drawString(Vector world_pos, std::string s, Alignment a, Color fg,
                  Color bg) const -> int;

  void setBackground(Color color);

  auto getHorizontalCells() const -> int;

  auto getVerticalCells() const -> int;

  auto getHorizontalPixels() const -> int;

  auto getVerticalPixels() const -> int;

  auto swapBuffers() -> int;

  auto getWindow() const -> sf::RenderWindow *;
};
}  // namespace df
