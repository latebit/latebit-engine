#pragma once

#include <SFML/Graphics.hpp>

namespace df {
enum Color {
  UNDEFINED_COLOR = -1,
  BLACK = 0,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
};

const Color COLOR_DEFAULT = WHITE;

auto fromColorString(std::string color_string) -> Color;

auto toSFColor(Color c) -> sf::Color;
}  // namespace df
