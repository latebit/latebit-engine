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

float charHeight();
float charWidth();

Vector spacesToPixels(Vector spaces);
Vector pixelsToSpaces(Vector pixels);

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
  static DisplayManager &getInstance();

  int startUp() override;

  void shutDown() override;

  int drawCh(Vector world_pos, char ch, Color fg) const;
  int drawCh(Vector world_pos, char ch, Color fg, Color bg) const;

  int drawString(Vector world_pos, std::string s, Alignment a, Color fg) const;
  int drawString(Vector world_pos, std::string s, Alignment a, Color fg,
                 Color bg) const;

  void setBackground(Color color);

  int getHorizontalCells() const;

  int getVerticalCells() const;

  int getHorizontalPixels() const;

  int getVerticalPixels() const;

  int swapBuffers();

  sf::RenderWindow *getWindow() const;
};
}  // namespace df
