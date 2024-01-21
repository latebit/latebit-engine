#include "SpriteParser.h"

#include "../lib/test.h"
#include "Colors.h"

using namespace df;

void makeFile(std::string filename, std::string content) {
  std::ofstream file(filename);
  file << content;
  file.close();
}

void SpriteParser_test() {
  Sprite* sprite =
    SpriteParser::parseSprite("test/fixtures/correct.txt", "test_sprite");

  assert("parses successfully", sprite != nullptr);
  if (sprite != nullptr) {
    assert_string("label is correct", sprite->getLabel(), "test_sprite");
    assert_int("frame count is correct", sprite->getFrameCount(), 2);
    assert_int("width is correct", sprite->getWidth(), 3);
    assert_int("height is correct", sprite->getHeight(), 4);
    assert_int("slowdown is correct", sprite->getSlowdown(), 2);
    assert_int("color is correct", sprite->getColor(), BLUE);
  }

  sprite =
    SpriteParser::parseSprite("test/fixtures/missing-frame.txt", "test_sprite");
  assert("returns null with missing frames", sprite == nullptr);

  sprite =
    SpriteParser::parseSprite("test/fixtures/wrong-width.txt", "test_sprite");
  assert("returns null with incorrect width", sprite == nullptr);

  sprite =
    SpriteParser::parseSprite("test/fixtures/wrong-height.txt", "test_sprite");
  assert("returns null with incorrect height", sprite == nullptr);

  makeFile("carriage.txt",
           "2\r\n3\r\n3\r\n2\r\nblue\r\n***\r\n***\r\n***\r\n***\r\n@@@\r\n@@@"
           "\r\n@@@\r\n@@@");
  sprite = SpriteParser::parseSprite("carriage.txt", "test_sprite");
  assert("handles \\r", sprite != nullptr);
  remove("carriage.txt");
}