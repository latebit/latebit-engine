#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

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

// Convert string to color
auto fromColorString(string colorName) -> Color;

// Converts a Color to a SFML Color
auto toSFColor(Color c) -> sf::Color;
}  // namespace df
