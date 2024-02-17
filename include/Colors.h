#pragma once

#include <SDL2/SDL_pixels.h>

using namespace std;

namespace lb {
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

const Color COLOR_DEFAULT = WHITE;

// Maps an hexadecimal character to a Color
auto fromHex(char c) -> Color;

// Converts a Color to a SDL Color
auto toSDLColor(Color c) -> SDL_Color;
}  // namespace lb
