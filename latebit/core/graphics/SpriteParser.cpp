#include "SpriteParser.h"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "./PNG.cpp"
#include "Colors.h"
#include "Keyframe.h"
#include "Sprite.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Parser.h"

using namespace std;

namespace lb {
auto SpriteParser::fromPNGFile(string filename, string label, int frames,
                               int duration) -> Sprite {
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
    Log.error("SpriteParser::fromPNGFile(): Could not read image data from %s",
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

  Sprite sprite(label, spriteWidth, height, duration, frames);

  for (int i = 0; i < frames; i++) {
    vector<Color> content;
    content.reserve(spriteWidth * height);

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < spriteWidth; x++) {
        content.push_back(decoder.getCellColor(x + i * spriteWidth, y));
      }
    }

    sprite.addFrame(Keyframe(spriteWidth, height, content));
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
  string version = getLine(stream);
  if (version != "#v0.1#") {
    if (version.at(0) == '#') {
      Log.error("Invalid header. Unsupported version %s", version.c_str());
      return {};
    } else {
      Log.error("Missing version header. Expected #v0.1#, got %s",
                version.c_str());
    }
    return {};
  }

  int frames = getNextNumber(stream);
  if (frames < 1 || frames > 64) {
    Log.error("Invalid frame count. %s",
              makeRangeValidationMessage(frames, 64).c_str());
    return {};
  }

  int width = getNextNumber(stream);
  if (width < 1 || width > 64) {
    Log.error("Invalid width. %s",
              makeRangeValidationMessage(width, 64).c_str());
    return {};
  }

  int height = getNextNumber(stream);
  if (height < 1 || height > 64) {
    Log.error("Invalid height. %s",
              makeRangeValidationMessage(height, 64).c_str());
    return {};
  }

  int duration = getNextNumber(stream);
  if (duration < 0 || duration > 255) {
    Log.error("Invalid frame duration. %s",
              makeRangeValidationMessage(height, 255).c_str());
    return {};
  }

  Sprite sprite(label, (uint8_t)width, (uint8_t)height, (uint8_t)duration,
                (uint8_t)frames);

  for (int i = 0; i < frames; i++) {
    if (!stream->good()) {
      Log.error(
        "SpriteParser::fromTextFile(): Unexpected end of file at frame %d", i);
      return {};
    }

    vector<Color> content;
    content.reserve(width * height);

    for (int j = 0; j < height; j++) {
      auto line = getLine(stream);
      if (line.size() != (uint8_t)width) {
        Log.error(
          "SpriteParser::fromTextFile(): Invalid line length %d for frame "
          "%d. "
          "Expected %d, got %d",
          i, j, width, line.length());
        return {};
      }

      for (char c : line) content.push_back(fromHex(c));
    }

    sprite.addFrame(Keyframe(width, height, content));
  }

  return sprite;
}

auto SpriteParser::toString(const Sprite &sprite) -> string {
  ostringstream stream;

  stream << "#v0.1#" << '\n';
  stream << (int)sprite.getFrameCount() << '\n';
  stream << (int)sprite.getWidth() << '\n';
  stream << (int)sprite.getHeight() << '\n';
  stream << (int)sprite.getDuration() << '\n';

  for (int i = 0; i < sprite.getFrameCount(); i++) {
    auto frame = sprite.getFrame(i);

    for (int y = 0; y < frame.getHeight(); y++) {
      for (int x = 0; x < frame.getWidth(); x++) {
        size_t index = x + y * frame.getWidth();
        stream << toHex(frame.getContent().at(index));
      }
      stream << '\n';
    }
  }

  return stream.str();
}

}  // namespace lb
