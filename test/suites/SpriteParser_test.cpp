#include "SpriteParser.h"

#include "../lib/test.h"
#include "Colors.h"

using namespace lb;
using namespace std;

void makeFile(string filename, string content) {
  ofstream file(filename);
  file << content;
  file.close();
}

void SpriteParser_test() {
  test("parseTextSprite/success", []() {
    Sprite sprite =
      SpriteParser::parseTextSprite("test/fixtures/correct.txt", "test_sprite");

    assert_string("label is correct", sprite.getLabel(), "test_sprite");
    assert_int("frame count is correct", sprite.getFrameCount(), 2);
    assert_int("width is correct", sprite.getWidth(), 3);
    assert_int("height is correct", sprite.getHeight(), 4);
    assert_int("slowdown is correct", sprite.getSlowdown(), 2);
    assert(
      "content of first frame is correct",
      sprite.getFrame(0).getContent() ==
        vector<Color>({Color::BLACK, Color::DARK_BLUE, Color::DARK_PURPLE,
                       Color::BLACK, Color::DARK_BLUE, Color::DARK_PURPLE,
                       Color::BLACK, Color::DARK_BLUE, Color::DARK_PURPLE,
                       Color::BLACK, Color::DARK_BLUE, Color::DARK_PURPLE}));
    assert(
      "content of second frame is correct",
      sprite.getFrame(1).getContent() ==
        vector<Color>({Color::DARK_GREEN, Color::BROWN, Color::DARK_GRAY,
                       Color::DARK_GREEN, Color::BROWN, Color::DARK_GRAY,
                       Color::DARK_GREEN, Color::BROWN, Color::DARK_GRAY,
                       Color::DARK_GREEN, Color::BROWN, Color::DARK_GRAY}));
  });

  test("parseTextSprite/validations", []() {
    auto sprite = SpriteParser::parseTextSprite(
      "test/fixtures/missing-frame.txt", "test_sprite");
    assert("returns null with missing frames", sprite == Sprite());

    sprite = SpriteParser::parseTextSprite("test/fixtures/wrong-width.txt",
                                           "test_sprite");
    assert("returns null with incorrect width", sprite == Sprite());

    sprite = SpriteParser::parseTextSprite("test/fixtures/wrong-height.txt",
                                           "test_sprite");
    assert("returns null with incorrect height", sprite == Sprite());
  });

  test("parseTextSprite/line endings", []() {
    makeFile("carriage.txt",
             "2\r\n3\r\n4\r\n2\r\n012\r\n012\r\n012\r\n012\r\nFFF\r\nFFF"
             "\r\nFFF\r\nFFF");
    auto sprite = SpriteParser::parseTextSprite("carriage.txt", "test_sprite");
    assert_string("label is correct", sprite.getLabel(), "test_sprite");
    assert_int("frame count is correct", sprite.getFrameCount(), 2);
    assert_int("width is correct", sprite.getWidth(), 3);
    assert_int("height is correct", sprite.getHeight(), 4);
    assert_int("slowdown is correct", sprite.getSlowdown(), 2);
    remove("carriage.txt");
  });

  test("parseImageSprite/single frame", []() {
    auto sprite = SpriteParser::parseImageSprite("test/fixtures/correct.png",
                                                 "correct", 1, 1);
    assert_string("label is correct", sprite.getLabel(), "correct");
    assert_int("frame count is correct", sprite.getFrameCount(), 1);
    assert_int("width is correct", sprite.getWidth(), 2);
    assert_int("height is correct", sprite.getHeight(), 2);
    assert_int("slowdown is correct", sprite.getSlowdown(), 1);
    assert("content of first frame is correct",
           sprite.getFrame(0).getContent() ==
             vector<Color>({DARK_BLUE, DARK_PURPLE, DARK_GREEN, BROWN}));
  });

  test("parseImageSprite/with transparency", []() {
    auto sprite = SpriteParser::parseImageSprite(
      "test/fixtures/correct-transparent.png", "transparent", 1, 1);
    assert(
      "detects transparency correctly",
      sprite.getFrame(0).getContent() ==
        vector<Color>({DARK_GREEN, DARK_GREEN, DARK_GREEN, UNDEFINED_COLOR}));
  });

  test("parseImageSprite/sprite sheet", []() {
    auto sprite =
      SpriteParser::parseImageSprite("test/fixtures/sheet.png", "sheet", 4, 1);
    assert_int("frame count is correct", sprite.getFrameCount(), 4);
    assert("content of first frame is correct",
           sprite.getFrame(0).getContent() == vector<Color>({YELLOW}));
    assert("content of second frame is correct",
           sprite.getFrame(1).getContent() == vector<Color>({ORANGE}));
    assert("content of third frame is correct",
           sprite.getFrame(2).getContent() == vector<Color>({RED}));
    assert("content of fourth frame is correct",
           sprite.getFrame(3).getContent() == vector<Color>({DARK_PURPLE}));
  });

  test("parseImageSprite/validations", []() {
    auto sprite = SpriteParser::parseImageSprite("test/fixtures/missing.png",
                                                 "missing", 1, 1);
    assert("returns null with missing file", sprite == Sprite());

    sprite = SpriteParser::parseImageSprite("test/fixtures/bigger-palette.png",
                                            "bigger-palette", 1, 1);
    assert("returns null with bigger palette", sprite == Sprite());

    sprite = SpriteParser::parseImageSprite("test/fixtures/not-indexed.png",
                                            "not-indexed", 1, 1);
    assert("returns null with not indexed images", sprite == Sprite());

    sprite = SpriteParser::parseImageSprite("test/fixtures/correct.txt",
                                            "correct", 1, 1);
    assert("returns null with a non PNG file", sprite == Sprite());

    sprite =
      SpriteParser::parseImageSprite("test/fixtures/sheet.png", "sheet", 5, 1);
    assert("returns null if width cannot be divided by frames",
           sprite == Sprite());
  });
}