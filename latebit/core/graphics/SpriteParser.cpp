#include "SpriteParser.h"

#include <fstream>
#include <string>
#include <vector>

#include "./PNG.cpp"
#include "Colors.h"
#include "Frame.h"
#include "Sprite.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Parser.h"

using namespace std;

namespace lb {
auto SpriteParser::fromPNGFile(string filename, string label, int frames,
                                    int slowdown) -> Sprite {
  auto decoder = PNGDecoder(filename);

  if (!decoder.canOpenFile()) {
    Log.error("SpriteParser::fromPNGFile(): Could not open file %s",
              filename.c_str());
    return {};
  }

  if (!decoder.isPNGFile()) {
    Log.error("SpriteParser::fromPNGFile(): %s is not a PNG file",
              filename.c_str());
    return {};
  }

  if (!decoder.canAllocateMemory()) {
    Log.error(
      "SpriteParser::fromPNGFile(): Could load %s. Unable to allocate "
      "memory",
      filename.c_str());
    return {};
  }

  if (decoder.decode() != 0) {
    Log.error(
      "SpriteParser::fromPNGFile(): Could not decode %s. Invalid PNG "
      "structure",
      filename.c_str());
    return {};
  }

  if (!decoder.is16ColorsPNG()) {
    Log.error(
      "SpriteParser::fromPNGFile(): Image is not palette-based. Only 16 "
      "colors PNGs are supported");
    return {};
  }

  if (decoder.readImage() != 0) {
    Log.error(
      "SpriteParser::fromPNGFile(): Could not read image data from %s",
      filename.c_str());
    return {};
  }

  int width = decoder.getWidth();
  int height = decoder.getHeight();

  if (width % frames != 0) {
    Log.error(
      "SpriteParser::fromPNGFile(): Image width %d cannot be divided in "
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

auto SpriteParser::fromTextFile(string filename, string label) -> Sprite {
  ifstream file(filename);

  if (!file.is_open()) {
    Log.error("SpriteParser::fromTextFile(): Could not open file %s",
              filename.c_str());
    return {};
  }

  return fromStream(&file, label);
}

auto SpriteParser::fromString(string str, string label) -> Sprite {
  istringstream stream(str);

  if (!stream.good()) {
    Log.error("SpriteParser::fromString(): Could not open string");
    return {};
  }

  return fromStream(&stream, label);
}

auto SpriteParser::fromStream(istream *stream, string label) -> Sprite {
  // Order of these lines matters! Do not change!
  uint8_t frames = stoi(getLine(stream));
  uint8_t width = stoi(getLine(stream));
  uint8_t height = stoi(getLine(stream));
  uint8_t slowdown = stoi(getLine(stream));

  Sprite sprite(label, width, height, slowdown, frames);

  for (int i = 0; i < frames; i++) {
    if (!stream->good()) {
      Log.error(
        "SpriteParser::fromTextFile(): Unexpected end of file at frame %d",
        i);
      return {};
    }

    vector<Color> content;
    content.reserve(width * height);

    for (int j = 0; j < height; j++) {
      auto line = getLine(stream);
      if (line.size() != width) {
        Log.error(
          "SpriteParser::fromTextFile(): Invalid line length %d for frame "
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
