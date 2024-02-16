#include "Colors.h"

#include <SFML/Graphics.hpp>

namespace df {

static const sf::Color Black = sf::Color(0, 0, 0);
static const sf::Color DarkBlue = sf::Color(29, 43, 83);
static const sf::Color DarkPurple = sf::Color(126, 37, 83);
static const sf::Color DarkGreen = sf::Color(0, 135, 81);
static const sf::Color Brown = sf::Color(171, 82, 54);
static const sf::Color DarkGray = sf::Color(95, 87, 79);
static const sf::Color LightGray = sf::Color(194, 195, 199);
static const sf::Color White = sf::Color(255, 241, 232);
static const sf::Color Red = sf::Color(255, 0, 77);
static const sf::Color Orange = sf::Color(255, 163, 0);
static const sf::Color Yellow = sf::Color(255, 236, 39);
static const sf::Color Green = sf::Color(0, 228, 54);
static const sf::Color Blue = sf::Color(41, 173, 255);
static const sf::Color Indigo = sf::Color(131, 118, 156);
static const sf::Color Pink = sf::Color(255, 119, 168);
static const sf::Color Peach = sf::Color(255, 204, 170);

auto toSFColor(Color c) -> sf::Color {
  switch (c) {
    case BLACK:
      return Black;
    case DARK_BLUE:
      return DarkBlue;
    case DARK_PURPLE:
      return DarkPurple;
    case DARK_GREEN:
      return DarkGreen;
    case BROWN:
      return Brown;
    case DARK_GRAY:
      return DarkGray;
    case LIGHT_GRAY:
      return LightGray;
    case WHITE:
      return White;
    case RED:
      return Red;
    case ORANGE:
      return Orange;
    case YELLOW:
      return Yellow;
    case GREEN:
      return Green;
    case BLUE:
      return Blue;
    case INDIGO:
      return Indigo;
    case PINK:
      return Pink;
    case PEACH:
      return Peach;
    case UNDEFINED_COLOR:
    default:
      return sf::Color::Transparent;
  }
}

auto fromHex(char c) -> Color {
  switch (c) {
    case '0':
      return BLACK;
    case '1':
      return DARK_BLUE;
    case '2':
      return DARK_PURPLE;
    case '3':
      return DARK_GREEN;
    case '4':
      return BROWN;
    case '5':
      return DARK_GRAY;
    case '6':
      return LIGHT_GRAY;
    case '7':
      return WHITE;
    case '8':
      return RED;
    case '9':
      return ORANGE;
    case 'a':
    case 'A':
      return YELLOW;
    case 'b':
    case 'B':
      return GREEN;
    case 'c':
    case 'C':
      return BLUE;
    case 'd':
    case 'D':
      return INDIGO;
    case 'e':
    case 'E':
      return PINK;
    case 'f':
    case 'F':
      return PEACH;
    default:
      return UNDEFINED_COLOR;
  }
}

}  // namespace df
