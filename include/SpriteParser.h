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
  static auto parseTextSprite(string filename, string label) -> Sprite;

  // Parses an indexed png file and returns a sprite with a given label
  static auto parseImageSprite(string filename, string label, int slowdown,
                               int frameCount) -> Sprite;
};
}  // namespace lb
