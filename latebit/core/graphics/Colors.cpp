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

auto toSDLColor(Color c) -> SDL_Color {
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
      return SDL_Color{0, 0, 0, 0};
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

auto toHex(Color c) -> char {
  switch (c) {
    case BLACK:
      return '0';
    case DARK_BLUE:
      return '1';
    case DARK_PURPLE:
      return '2';
    case DARK_GREEN:
      return '3';
    case BROWN:
      return '4';
    case DARK_GRAY:
      return '5';
    case LIGHT_GRAY:
      return '6';
    case WHITE:
      return '7';
    case RED:
      return '8';
    case ORANGE:
      return '9';
    case YELLOW:
      return 'A';
    case GREEN:
      return 'B';
    case BLUE:
      return 'C';
    case INDIGO:
      return 'D';
    case PINK:
      return 'E';
    case PEACH:
      return 'F';
    case UNDEFINED_COLOR:
    default:
      return '-';
  }
}

}  // namespace lb
