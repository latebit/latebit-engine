#pragma once

#include <SDL2/SDL_pixels.h>

namespace lb {

namespace Color {
enum Color {
  UNDEFINED_COLOR = -1,
  BLACK = 0,
  DARK_BLUE,
  DARK_PURPLE,
  DARK_GREEN,
  BROWN,
  DARK_GRAY,
  LIGHT_GRAY,
  WHITE,
  RED,
  ORANGE,
  YELLOW,
  GREEN,
  BLUE,
  INDIGO,
  PINK,
  PEACH,
};
}

const Color::Color COLOR_DEFAULT = Color::WHITE;

// Maps an hexadecimal character to a Color
auto fromHex(char c) -> Color::Color;

// Maps a Color to an hexadecimal character
auto toHex(Color::Color c) -> char;

// Converts a Color to a SDL Color
auto toSDLColor(Color::Color c) -> SDL_Color;
}  // namespace lb
