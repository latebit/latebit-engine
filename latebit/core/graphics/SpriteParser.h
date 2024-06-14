#pragma once

#include <fstream>
#include <string>

#include "Colors.h"
#include "Sprite.h"

using namespace std;

namespace lb {
class SpriteParser {
 private:
  static auto fromStream(istream *stream, string label) -> Sprite;

 public:
  // Parses a text file and returns a sprite with a given label
  static auto fromTextFile(string filename, string label) -> Sprite;

  // Parses a string and returns a sprite with a given label
  static auto fromString(const string str, string label) -> Sprite;

  // Load a 16-colors PNG sprite by its filename and associate it with a label
  // The PNG can have multiple frames, in which case we will assume they all are
  // on the same line and divide the image width by the number of frames
  static auto fromPNGFile(string filename, string label, int frames,
                               int slowdown) -> Sprite;

  // Converts a sprite to a string
  static auto toString(const Sprite &sprite) -> string;
};
}  // namespace lb
