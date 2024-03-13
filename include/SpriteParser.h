#pragma once

#include <fstream>
#include <string>

#include "Colors.h"
#include "Sprite.h"

using namespace std;

namespace lb {
class SpriteParser {
 private:
  static auto parseHeader(ifstream *file, int *frames, int *width, int *height,
                          int *slowdown, Color *color) -> int;

 public:
  // Parses a file and returns a sprite with a given label
  static auto parseSprite(string filename, string label) -> Sprite;
};
}  // namespace lb
