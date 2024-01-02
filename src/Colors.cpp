#include "Colors.h"

#include <SFML/Graphics.hpp>

sf::Color df::toSFColor(Color c) {
  {
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
}