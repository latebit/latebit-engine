#include "../include/SpriteParser.h"

#include "../include/Colors.h"
#include "test.h"

void makeFile(std::string filename, std::string content) {
  std::ofstream file(filename);
  file << content;
  file.close();
}

auto SpriteParser_test() -> int {
  int result = 0;

  std::string filename = "sprite.txt";
  makeFile(filename,
           "2\n3\n4\n2\nblue\n***\n***\n***\n***\n@@@\n@@@\n@@@\n@@@");
  df::Sprite* sprite = df::SpriteParser::parseSprite(filename, "test_sprite");

  result += assert("parseSprite returns non-null", sprite != nullptr);
  if (sprite != nullptr) {
    result +=
        assert_string("label is correct", sprite->getLabel(), "test_sprite");
    result += assert_int("frame count is correct", sprite->getFrameCount(), 2);
    result += assert_int("width is correct", sprite->getWidth(), 3);
    result += assert_int("height is correct", sprite->getHeight(), 4);
    result += assert_int("slowdown is correct", sprite->getSlowdown(), 2);
    result += assert_int("color is correct", sprite->getColor(), df::BLUE);
  }

  remove(filename.c_str());

  makeFile(filename, "2\n3\n4\n2\nblue\n***\n***\n***\n***");
  sprite = df::SpriteParser::parseSprite(filename, "test_sprite");
  result += assert("returns null with missing frames", sprite == nullptr);
  remove(filename.c_str());

  makeFile(filename,
           "2\n2\n4\n2\nblue\n***\n***\n***\n***\n@@@\n@@@\n@@@\n@@@");
  sprite = df::SpriteParser::parseSprite(filename, "test_sprite");
  result += assert("returns null with incorrect width", sprite == nullptr);
  remove(filename.c_str());

  makeFile(filename,
           "2\n3\n5\n2\nblue\n***\n***\n***\n***\n@@@\n@@@\n@@@\n@@@");
  sprite = df::SpriteParser::parseSprite(filename, "test_sprite");
  result += assert("returns null with incorrect height", sprite == nullptr);
  remove(filename.c_str());

  makeFile(filename,
           "2\r\n3\r\n3\r\n2\r\nblue\r\n***\r\n***\r\n***\r\n***\r\n@@@\r\n@@@"
           "\r\n@@@\r\n@@@");
  sprite = df::SpriteParser::parseSprite(filename, "test_sprite");
  result += assert("handles \\r", sprite != nullptr);
  remove(filename.c_str());

  return result;
}