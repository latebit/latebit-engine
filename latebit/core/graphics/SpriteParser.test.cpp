#include "latebit/core/graphics/SpriteParser.h"

#include <filesystem>

#include "../../../test/lib/test.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/utils/Logger.h"

using namespace lb;
using namespace std;

const Sprite NULL_SPRITE = Sprite();

auto main() -> int {
  Log.setDestination(STDOUT);
  suite("when parsing text sprite", []() {
    test("success", []() {
      Sprite sprite = SpriteParser::fromTextFile(
        FIXTURES_FOLDER + "/correct.lbspr", "test_sprite");

      assertEq("label is correct", sprite.getLabel(), "test_sprite");
      assertEq("frame count is correct", sprite.getFrameCount(), 2);
      assertEq("width is correct", sprite.getWidth(), 3);
      assertEq("height is correct", sprite.getHeight(), 4);
      assertEq("duration is correct", sprite.getDuration(), 2);
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

    test("validates the frames", []() {
      auto missing = SpriteParser::fromTextFile(
        FIXTURES_FOLDER + "/missing-frame.lbspr", "test_sprite");
      assert("returns null with missing frames", missing == NULL_SPRITE);

      auto width = SpriteParser::fromTextFile(
        FIXTURES_FOLDER + "/wrong-width.lbspr", "test_sprite");
      assert("returns null with incorrect width", width == NULL_SPRITE);

      auto height = SpriteParser::fromTextFile(
        FIXTURES_FOLDER + "/wrong-height.lbspr", "test_sprite");
      assert("returns null with incorrect height", height == NULL_SPRITE);
    });

    test("validates the headers", []() {
      const string lowFrames = "#v0.1#\n0\n1\n1\n1\n0";
      assert("fails with low frames",
             SpriteParser::fromString(lowFrames, "") == NULL_SPRITE);

      const string highFrames = "#v0.1#\n65\n1\n1\n1\n0";
      assert("fails with high frames",
             SpriteParser::fromString(highFrames, "") == NULL_SPRITE);

      const string lowWidth = "#v0.1#\n1\n0\n1\n1\n0";
      assert("fails with low width",
             SpriteParser::fromString(lowWidth, "") == NULL_SPRITE);

      const string highWidth = "#v0.1#\n1\n65\n1\n1\n0";
      assert("fails with high width",
             SpriteParser::fromString(highWidth, "") == NULL_SPRITE);

      const string lowHeight = "#v0.1#\n1\n1\n0\n1\n0";
      assert("fails with low height",
             SpriteParser::fromString(lowHeight, "") == NULL_SPRITE);

      const string highHeight = "#v0.1#\n1\n1\n65\n1\n0";
      assert("fails with high height",
             SpriteParser::fromString(highHeight, "") == NULL_SPRITE);

      const string withStopAnimation = "#v0.1#\n1\n1\n1\n0\n1";
      const Sprite result = SpriteParser::fromString(withStopAnimation, "");
      auto frames = vector<Keyframe>();
      frames.push_back(Keyframe(1, 1, vector<Color>({Color::DARK_BLUE})));
      Sprite expected = Sprite("", 1, 1, 0, frames);
      assert("allows stopAnimation", result == expected);

      const string lowDuration = "#v0.1#\n1\n1\n1\n-1\n0";
      assert("fails with low duration",
             SpriteParser::fromString(highHeight, "") == NULL_SPRITE);

      const string highDuration = "#v0.1#\n1\n1\n1\n256\n0";
      assert("fails with high duration",
             SpriteParser::fromString(highHeight, "") == NULL_SPRITE);
    });

    test("line endings", []() {
      const string text =
        "#v0.1#\r\n"
        "2\r\n3\r\n4\r\n2\r\n"
        "012\r\n012\r\n012\r\n012\r\nFFF\r\nFFF\r\nFFF\r\nFFF";
      auto sprite = SpriteParser::fromString(text, "test_sprite");
      assertEq("label is correct", sprite.getLabel(), "test_sprite");
      assertEq("frame count is correct", sprite.getFrameCount(), 2);
      assertEq("width is correct", sprite.getWidth(), 3);
      assertEq("height is correct", sprite.getHeight(), 4);
      assertEq("duration is correct", sprite.getDuration(), 2);
    });
  });

  suite("when parsing image sprite", []() {
    test("single frame", []() {
      auto sprite = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/correct.png",
                                              "correct", 1, 1);
      assertEq("label is correct", sprite.getLabel(), "correct");
      assertEq("frame count is correct", sprite.getFrameCount(), 1);
      assertEq("width is correct", sprite.getWidth(), 2);
      assertEq("height is correct", sprite.getHeight(), 2);
      assertEq("duration is correct", sprite.getDuration(), 1);
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
      auto missing = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/missing.png",
                                               "missing", 1, 1);
      assert("returns null with missing file", missing == NULL_SPRITE);

      auto palette = SpriteParser::fromPNGFile(
        FIXTURES_FOLDER + "/bigger-palette.png", "bigger-palette", 1, 1);
      assert("returns null with bigger palette", palette == NULL_SPRITE);

      auto index = SpriteParser::fromPNGFile(
        FIXTURES_FOLDER + "/not-indexed.png", "not-indexed", 1, 1);
      assert("returns null with not indexed images", index == NULL_SPRITE);

      auto correct = SpriteParser::fromPNGFile(
        FIXTURES_FOLDER + "/correct.lbspr", "correct", 1, 1);
      assert("returns null with a non PNG file", correct == NULL_SPRITE);

      auto sheet = SpriteParser::fromPNGFile(FIXTURES_FOLDER + "/sheet.png",
                                             "sheet", 5, 1);
      assert("returns null if width cannot be divided by frames",
             sheet == NULL_SPRITE);
    });
  });

  suite("when converting sprite to text", []() {
    test("single frame", []() {
      auto frames = vector<Keyframe>();
      frames.push_back(Keyframe(1, 1, vector<Color>({BLACK})));
      auto sprite = Sprite("single", 1, 1, 1, frames);
      auto text = SpriteParser::toString(sprite);
      auto expected = "#v0.1#\n1\n1\n1\n1\n0\n";
      assertEq("text is correct", text, expected);
    });

    test("sprite sheet", []() {
      auto frames = vector<Keyframe>();
      frames.push_back(Keyframe(1, 1, vector<Color>({BLACK})));
      frames.push_back(Keyframe(1, 1, vector<Color>({DARK_BLUE})));
      auto sprite = Sprite("sheet", 1, 1, 1, frames);
      auto text = SpriteParser::toString(sprite);
      auto expected = "#v0.1#\n2\n1\n1\n1\n0\n1\n";
      assertEq("text is correct", text, expected);
    });
  });

  return report();
}