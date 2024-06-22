#include "Colors.h"

namespace lb {

static const SDL_Color Black = {0, 0, 0, 255};
static const SDL_Color DarkBlue = {29, 43, 83, 255};
static const SDL_Color DarkPurple = {126, 37, 83, 255};
static const SDL_Color DarkGreen = {0, 135, 81, 255};
static const SDL_Color Brown = {171, 82, 54, 255};
static const SDL_Color DarkGray = {95, 87, 79, 255};
static const SDL_Color LightGray = {194, 195, 199, 255};
static const SDL_Color White = {255, 241, 232, 255};
static const SDL_Color Red = {255, 0, 77, 255};
static const SDL_Color Orange = {255, 163, 0, 255};
static const SDL_Color Yellow = {255, 236, 39, 255};
static const SDL_Color Green = {0, 228, 54, 255};
static const SDL_Color Blue = {41, 173, 255, 255};
static const SDL_Color Indigo = {131, 118, 156, 255};
static const SDL_Color Pink = {255, 119, 168, 255};
static const SDL_Color Peach = {255, 204, 170, 255};

auto toSDLColor(Color::Color c) -> SDL_Color {
  switch (c) {
    case Color::BLACK:
      return Black;
    case Color::DARK_BLUE:
      return DarkBlue;
    case Color::DARK_PURPLE:
      return DarkPurple;
    case Color::DARK_GREEN:
      return DarkGreen;
    case Color::BROWN:
      return Brown;
    case Color::DARK_GRAY:
      return DarkGray;
    case Color::LIGHT_GRAY:
      return LightGray;
    case Color::WHITE:
      return White;
    case Color::RED:
      return Red;
    case Color::ORANGE:
      return Orange;
    case Color::YELLOW:
      return Yellow;
    case Color::GREEN:
      return Green;
    case Color::BLUE:
      return Blue;
    case Color::INDIGO:
      return Indigo;
    case Color::PINK:
      return Pink;
    case Color::PEACH:
      return Peach;
    case Color::UNDEFINED_COLOR:
    default:
      return SDL_Color{0, 0, 0, 0};
  }
}

auto fromHex(char c) -> Color::Color {
  switch (c) {
    case '0':
      return Color::BLACK;
    case '1':
      return Color::DARK_BLUE;
    case '2':
      return Color::DARK_PURPLE;
    case '3':
      return Color::DARK_GREEN;
    case '4':
      return Color::BROWN;
    case '5':
      return Color::DARK_GRAY;
    case '6':
      return Color::LIGHT_GRAY;
    case '7':
      return Color::WHITE;
    case '8':
      return Color::RED;
    case '9':
      return Color::ORANGE;
    case 'a':
    case 'A':
      return Color::YELLOW;
    case 'b':
    case 'B':
      return Color::GREEN;
    case 'c':
    case 'C':
      return Color::BLUE;
    case 'd':
    case 'D':
      return Color::INDIGO;
    case 'e':
    case 'E':
      return Color::PINK;
    case 'f':
    case 'F':
      return Color::PEACH;
    default:
      return Color::UNDEFINED_COLOR;
  }
}

auto toHex(Color::Color c) -> char {
  switch (c) {
    case Color::BLACK:
      return '0';
    case Color::DARK_BLUE:
      return '1';
    case Color::DARK_PURPLE:
      return '2';
    case Color::DARK_GREEN:
      return '3';
    case Color::BROWN:
      return '4';
    case Color::DARK_GRAY:
      return '5';
    case Color::LIGHT_GRAY:
      return '6';
    case Color::WHITE:
      return '7';
    case Color::RED:
      return '8';
    case Color::ORANGE:
      return '9';
    case Color::YELLOW:
      return 'A';
    case Color::GREEN:
      return 'B';
    case Color::BLUE:
      return 'C';
    case Color::INDIGO:
      return 'D';
    case Color::PINK:
      return 'E';
    case Color::PEACH:
      return 'F';
    case Color::UNDEFINED_COLOR:
    default:
      return '-';
  }
}

}  // namespace lb
