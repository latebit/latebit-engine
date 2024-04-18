#include "SpriteParser.h"

#include <png.h>

#include <fstream>
#include <string>
#include <vector>

#include "./PNG.cpp"
#include "Colors.h"
#include "Frame.h"
#include "Sprite.h"
#include "utils/Logger.h"
#include "utils/Parser.h"

using namespace std;

namespace lb {
auto SpriteParser::parseImageSprite(string filename, string label, int frames,
                                    int slowdown) -> Sprite {
  auto decoder = PNGDecoder(filename);

  if (!decoder.canOpenFile()) {
    Log.error("SpriteParser::parseImageSprite(): Could not open file %s",
              filename.c_str());
    return {};
  }

  if (!decoder.isPNGFile()) {
    Log.error("SpriteParser::parseImageSprite(): %s is not a PNG file",
              filename.c_str());
    return {};
  }

  if (!decoder.canAllocateMemory()) {
    Log.error(
      "SpriteParser::parseImageSprite(): Could load %s. Unable to allocate "
      "memory",
      filename.c_str());
    return {};
  }

  if (decoder.decode() != 0) {
    Log.error(
      "SpriteParser::parseImageSprite(): Could not decode %s. Invalid PNG "
      "structure",
      filename.c_str());
    return {};
  }

  if (!decoder.is16ColorsPNG()) {
    Log.error(
      "SpriteParser::parseImageSprite(): Image is not palette-based. Only 16 "
      "colors PNGs are supported");
    return {};
  }

  if (decoder.readImage() != 0) {
    Log.error(
      "SpriteParser::parseImageSprite(): Could not read image data from %s",
      filename.c_str());
    return {};
  }

  int width = decoder.getWidth();
  int height = decoder.getHeight();

  if (width % frames != 0) {
    Log.error(
      "SpriteParser::parseImageSprite(): Image width %d cannot be divided in "
      "%d equal frames",
      width, frames);
    return {};
  }

  // We assume the sprite is horizontal
  int spriteWidth = width / frames;

  Sprite sprite(label, spriteWidth, height, slowdown, frames);

  for (int i = 0; i < frames; i++) {
    vector<Color> content;
    content.reserve(spriteWidth * height);

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < spriteWidth; x++) {
        content.push_back(decoder.getCellColor(x + i * spriteWidth, y));
      }
    }

    sprite.addFrame(Frame(spriteWidth, height, content));
  }

  return sprite;
}

auto SpriteParser::parseTextSprite(string filename, string label) -> Sprite {
  ifstream file(filename);

  if (!file.is_open()) {
    Log.error("SpriteParser::parseTextSprite(): Could not open file %s",
              filename.c_str());
    return {};
  }

  // Order of these lines matters! Do not change!
  int frames = stoi(getLine(&file));
  int width = stoi(getLine(&file));
  int height = stoi(getLine(&file));
  int slowdown = stoi(getLine(&file));

  Sprite sprite(label, width, height, slowdown, frames);

  for (int i = 0; i < frames; i++) {
    if (!file.good()) {
      Log.error(
        "SpriteParser::parseTextSprite(): Unexpected end of file at frame %d",
        i);
      return {};
    }

    vector<Color> content;
    content.reserve(width * height);

    for (int j = 0; j < height; j++) {
      auto line = getLine(&file);
      if (line.size() != width) {
        Log.error(
          "SpriteParser::parseTextSprite(): Invalid line length %d for frame "
          "%d. "
          "Expected %d, got %d",
          i, j, width, line.length());
        return {};
      }

      for (char c : line) content.push_back(fromHex(c));
    }

    sprite.addFrame(Frame(width, height, content));
  }

  return sprite;
}

}  // namespace lb
