#pragma once

#include <fstream>
#include <string>

#include "Colors.h"
#include "Frame.h"
#include "Sprite.h"

namespace df {
class SpriteParser {
 private:
  static std::string getLine(std::ifstream *file_stream);
  static int parseHeader(std::ifstream *file_stream, int *frames, int *width,
                         int *height, int *slowdown, Color *color);

 public:
  static Sprite *parseSprite(std::string filename, std::string label);
};
}  // namespace df

