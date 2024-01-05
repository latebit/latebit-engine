#include "Colors.h"

#include <SFML/Graphics.hpp>
#include <string>

namespace df {
sf::Color toSFColor(Color c) {
  switch (c) {
    case BLACK:
      return sf::Color::Black;
    case BLUE:
      return sf::Color::Blue;
    case RED:
      return sf::Color::Red;
    case YELLOW:
      return sf::Color::Yellow;
    case GREEN:
      return sf::Color::Green;
    case MAGENTA:
      return sf::Color::Magenta;
    case CYAN:
      return sf::Color::Cyan;
    case WHITE:
      return sf::Color::White;
    case UNDEFINED_COLOR:
    default:
      return toSFColor(COLOR_DEFAULT);
  }
}

Color fromColorString(std::string s) {
  if (s == "black")
    return BLACK;
  else if (s == "blue")
    return BLUE;
  else if (s == "red")
    return RED;
  else if (s == "yellow")
    return YELLOW;
  else if (s == "green")
    return GREEN;
  else if (s == "magenta")
    return MAGENTA;
  else if (s == "cyan")
    return CYAN;
  else if (s == "white")
    return WHITE;
  else
    return UNDEFINED_COLOR;
}

}  // namespace df
