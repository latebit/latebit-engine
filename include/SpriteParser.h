#pragma once

#include <fstream>
#include <string>

#include "Colors.h"
#include "Frame.h"
#include "Sprite.h"

using namespace std;

namespace lb {
class SpriteParser {
 private:
  static auto getLine(ifstream *file_stream) -> string;
  static auto parseHeader(ifstream *file_stream, int *frames, int *width,
                          int *height, int *slowdown, Color *color) -> int;

 public:
  // Parses a file and returns a sprite with a given label
  static auto parseSprite(string filename, string label) -> Sprite;
};
}  // namespace lb
