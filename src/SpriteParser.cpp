#include "SpriteParser.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Colors.h"
#include "LogManager.h"
#include "Sprite.h"

namespace df {
std::string SpriteParser::getLine(std::ifstream* file_stream) {
  std::string line;
  getline(*file_stream, line);
  line.erase(std::remove_if(line.begin(), line.end(),
                            [](char c) { return c == '\r'; }),
             line.end());
  return line;
}

int SpriteParser::parseHeader(std::ifstream* file_stream, int* frames,
                              int* width, int* height, int* slowdown,
                              Color* color) {
  for (int i = 0; i < 5; i++) {
    auto line = getLine(file_stream);
    switch (i) {
      case 0:
        *frames = std::stoi(line);
        break;
      case 1:
        *width = std::stoi(line);
        break;
      case 2:
        *height = std::stoi(line);
        break;
      case 3:
        *slowdown = std::stoi(line);
        break;
      case 4:
        *color = fromColorString(line);
        break;
      default:
        break;
    }
  }

  return 0;
}

Sprite* SpriteParser::parseSprite(std::string filename, std::string label) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    LM.writeLog("SpriteParser::parseSprite(): Could not open file '%s'.",
                filename.c_str());
    return nullptr;
  }

  int frameCount, width, height, slowdown;
  Color color;

  if (parseHeader(&file, &frameCount, &width, &height, &slowdown, &color) !=
      0) {
    LM.writeLog("SpriteParser::parseSprite(): Could not parse header.");
    return nullptr;
  }

  Sprite* sprite = new Sprite(frameCount);
  sprite->setLabel(label);
  sprite->setWidth(width);
  sprite->setHeight(height);
  sprite->setSlowdown(slowdown);
  sprite->setColor(color);

  for (int i = 0; i < frameCount; i++) {
    if (!file.good()) {
      LM.writeLog(
          "SpriteParser::parseSprite(): Unexpected end of file at frame "
          "%d.",
          i);
      delete sprite;
      return nullptr;
    }

    std::string frameString;

    for (int j = 0; j < height; j++) {
      auto line = getLine(&file);
      if (line.size() != width) {
        LM.writeLog(
            "SpriteParser::parseSpriteBody(): Invalid line length "
            "for frame %d, line %d, expected %d got %d.",
            i, j, width, line.length());
        delete sprite;
        return nullptr;
      }

      frameString += line;
    }

    sprite->addFrame(Frame(width, height, frameString));
  }

  return sprite;
}

}  // namespace df
