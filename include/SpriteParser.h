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

  // Load a 16-colors PNG sprite by its filename and associate it with a label
  // The PNG can have multiple frames, in which case we will assume they all are
  // on the same line and divide the image width by the number of frames
  static auto parseImageSprite(string filename, string label, int frames,
                               int slowdown) -> Sprite;
};
}  // namespace lb
