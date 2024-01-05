#ifndef __COLORS_H__
#define __COLORS_H__

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

Color fromColorString(std::string color_string);

sf::Color toSFColor(Color c);
}  // namespace df

#endif