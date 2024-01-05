#ifndef __SPRITEPARSER_H__
#define __SPRITEPARSER_H__

#include <fstream>
#include <string>

#include "Colors.h"
#include "Frame.h"
#include "Sprite.h"

namespace df {
class SpriteParser {
 private:
  static int parseHeader(std::ifstream *p_file_stream, int *frames, int *width,
                         int *height, int *slowdown, Color *color);

 public:
  static Sprite *parseSprite(std::string filename, std::string label);
};
}  // namespace df

#endif