#include "latebit/core/graphics/SpriteParser.h"

#include <filesystem>

#include "../../../test/lib/test.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/utils/Logger.h"

using namespace lb;
using namespace std;

void makeFile(string filename, string content) {
  ofstream file(filename);
  file << content;
  file.close();
}

auto main() -> int {
  // print working directory
  std::cout << "Current working directory: " << filesystem::current_path()
            << std::endl;

  Log.setDestination(STDOUT);
  suite("parse text sprite", []() {
    test("success", []() {
      Sprite sprite = SpriteParser::fromTextFile(
        FIXTURES_FOLDER + "/correct.txt", "test_sprite");

      assertEq("label is correct", sprite.getLabel(), "test_sprite");
      assertEq("frame count is correct", sprite.getFrameCount(), 2);
      assertEq("width is correct", sprite.getWidth(), 3);
      assertEq("height is correct", sprite.getHeight(), 4);
      assertEq("slowdown is correct", sprite.getSlowdown(), 2);
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

    test("validations", []() {
      auto sprite = SpriteParser::fromTextFile(
        FIXTURES_FOLDER + "/missing-frame.txt", "test_sprite");
      assert("returns null with missing frames", sprite == Sprite());

      sprite = SpriteParser::fromTextFile(FIXTURES_FOLDER + "/wrong-width.txt",
                                          "test_sprite");
      assert("returns null with incorrect width", sprite == Sprite());

      sprite = SpriteParser::fromTextFile(FIXTURES_FOLDER + "/wrong-height.txt",
                                          "test_sprite");
      assert("returns null with incorrect height", sprite == Sprite());
    });

    test("line endings", []() {
      makeFile("carriage.txt",
               "2\r\n3\r\n4\r\n2\r\n012\r\n012\r\n012\r\n012\r\nFFF\r\nFFF"
               "\r\nFFF\r\nFFF");
      auto sprite = SpriteParser::fromTextFile("carriage.txt", "test_sprite");
      assertEq("label is correct", sprite.getLabel(), "test_sprite");
      assertEq("frame count is correct", sprite.getFrameCount(), 2);
      assertEq("width is correct", sprite.getWidth(), 3);
      assertEq("height is correct", sprite.getHeight(), 4);
      assertEq("slowdown is correct", sprite.getSlowdown(), 2);
      remove("carriage.txt");
    });
  });

  suite("parse image sprite", []() {
    test("single frame", []() {
      auto sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/correct.png",
                                              "correct", 1, 1);
      assertEq("label is correct", sprite.getLabel(), "correct");
      assertEq("frame count is correct", sprite.getFrameCount(), 1);
      assertEq("width is correct", sprite.getWidth(), 2);
      assertEq("height is correct", sprite.getHeight(), 2);
      assertEq("slowdown is correct", sprite.getSlowdown(), 1);
      assert("content of first frame is correct",
             sprite.getFrame(0).getContent() ==
               vector<Color>({DARK_BLUE, DARK_PURPLE, DARK_GREEN, BROWN}));
    });

    test("with transparency", []() {
      auto sprite = SpriteParser::fromPNGFile(
        FIXTURES_FOLDER + "/correct-transparent.png", "transparent", 1, 1);
      assert(
        "detects transparency correctly",
        sprite.getFrame(0).getContent() ==
          vector<Color>({DARK_GREEN, DARK_GREEN, DARK_GREEN, UNDEFINED_COLOR}));
    });

    test("sprite sheet", []() {
      auto sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/sheet.png",
                                              "sheet", 4, 1);
      assertEq("frame count is correct", sprite.getFrameCount(), 4);
      assert("content of first frame is correct",
             sprite.getFrame(0).getContent() == vector<Color>({YELLOW}));
      assert("content of second frame is correct",
             sprite.getFrame(1).getContent() == vector<Color>({ORANGE}));
      assert("content of third frame is correct",
             sprite.getFrame(2).getContent() == vector<Color>({RED}));
      assert("content of fourth frame is correct",
             sprite.getFrame(3).getContent() == vector<Color>({DARK_PURPLE}));
    });

    test("validations", []() {
      auto sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/missing.png",
                                              "missing", 1, 1);
      assert("returns null with missing file", sprite == Sprite());

      sprite = SpriteParser::fromPNGFile(
        FIXTURES_FOLDER + "/bigger-palette.png", "bigger-palette", 1, 1);
      assert("returns null with bigger palette", sprite == Sprite());

      sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/not-indexed.png",
                                         "not-indexed", 1, 1);
      assert("returns null with not indexed images", sprite == Sprite());

      sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/correct.txt",
                                         "correct", 1, 1);
      assert("returns null with a non PNG file", sprite == Sprite());

      sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/sheet.png",
                                         "sheet", 5, 1);
      assert("returns null if width cannot be divided by frames",
             sprite == Sprite());
    });
  });

  return report();
}