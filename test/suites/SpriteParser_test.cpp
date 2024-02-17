#include "SpriteParser.h"

#include "../lib/test.h"

using namespace lb;
using namespace std;

void makeFile(string filename, string content) {
  ofstream file(filename);
  file << content;
  file.close();
}

void SpriteParser_test() {
  Sprite sprite =
    SpriteParser::parseSprite("test/fixtures/correct.txt", "test_sprite");

  assert_string("label is correct", sprite.getLabel(), "test_sprite");
  assert_int("frame count is correct", sprite.getFrameCount(), 2);
  assert_int("width is correct", sprite.getWidth(), 3);
  assert_int("height is correct", sprite.getHeight(), 4);
  assert_int("slowdown is correct", sprite.getSlowdown(), 2);

  test("validations", []() {
    auto sprite = SpriteParser::parseSprite("test/fixtures/missing-frame.txt",
                                            "test_sprite");
    assert("returns null with missing frames", sprite == Sprite());

    sprite =
      SpriteParser::parseSprite("test/fixtures/wrong-width.txt", "test_sprite");
    assert("returns null with incorrect width", sprite == Sprite());

    sprite = SpriteParser::parseSprite("test/fixtures/wrong-height.txt",
                                       "test_sprite");
    assert("returns null with incorrect height", sprite == Sprite());
  });

  test("handles carriage return", []() {
    makeFile("carriage.txt",
             "2\r\n3\r\n4\r\n2\r\n012\r\n012\r\n012\r\n012\r\nFFF\r\nFFF"
             "\r\nFFF\r\nFFF");
    auto sprite = SpriteParser::parseSprite("carriage.txt", "test_sprite");
    assert_string("label is correct", sprite.getLabel(), "test_sprite");
    assert_int("frame count is correct", sprite.getFrameCount(), 2);
    assert_int("width is correct", sprite.getWidth(), 3);
    assert_int("height is correct", sprite.getHeight(), 4);
    assert_int("slowdown is correct", sprite.getSlowdown(), 2);
    remove("carriage.txt");
  });
}