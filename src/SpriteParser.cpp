#include "SpriteParser.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Colors.h"
#include "LogManager.h"
#include "Sprite.h"

namespace df {
int SpriteParser::parseHeader(std::ifstream* p_file_stream, int* p_frames,
                              int* p_width, int* p_height, int* p_slowdown,
                              Color* p_color) {
  for (int i = 0; i < 5; i++) {
    std::string line;
    getline(*p_file_stream, line);
    switch (i) {
      case 0:
        *p_frames = std::stoi(line);
        break;
      case 1:
        *p_width = std::stoi(line);
        break;
      case 2:
        *p_height = std::stoi(line);
        break;
      case 3:
        *p_slowdown = std::stoi(line);
        break;
      case 4:
        *p_color = fromColorString(line);
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
      std::string line;
      getline(file, line);

      if (line.size() != width) {
        LM.writeLog(
            "SpriteParser::parseSpriteBody(): Invalid line length (%d) "
            "for frame %d, expected %d.",
            line.size(), i, width);
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
