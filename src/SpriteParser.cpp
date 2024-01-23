#include "SpriteParser.h"

#include <fstream>
#include <string>

#include "Colors.h"
#include "LogManager.h"
#include "Sprite.h"

using namespace std;

namespace df {
auto SpriteParser::getLine(ifstream* file) -> string {
  string line;
  getline(*file, line);
  line.erase(
    remove_if(line.begin(), line.end(), [](char c) { return c == '\r'; }),
    line.end());
  return line;
}

auto SpriteParser::parseSprite(string filename, string label) -> Sprite {
  ifstream file(filename);

  if (!file.is_open()) {
    LM.writeLog("SpriteParser::parseSprite(): Could not open file '%s'.",
                filename.c_str());
    return {};
  }

  // Order of these lines matters! Do not change!
  int frames = stoi(getLine(&file));
  int width = stoi(getLine(&file));
  int height = stoi(getLine(&file));
  int slowdown = stoi(getLine(&file));
  Color color = fromColorString(getLine(&file));

  Sprite sprite(frames);
  sprite.setLabel(label);
  sprite.setWidth(width);
  sprite.setHeight(height);
  sprite.setSlowdown(slowdown);
  sprite.setColor(color);

  for (int i = 0; i < frames; i++) {
    if (!file.good()) {
      LM.writeLog(
        "SpriteParser::parseSprite(): Unexpected end of file at frame "
        "%d.",
        i);
      return {};
    }

    string frameString;

    for (int j = 0; j < height; j++) {
      auto line = getLine(&file);
      if (line.size() != width) {
        LM.writeLog(
          "SpriteParser::parseSpriteBody(): Invalid line length "
          "for frame %d, line %d, expected %d got %d.",
          i, j, width, line.length());
        return {};
      }

      frameString += line;
    }

    sprite.addFrame(Frame(width, height, frameString));
  }

  return sprite;
}

}  // namespace df
