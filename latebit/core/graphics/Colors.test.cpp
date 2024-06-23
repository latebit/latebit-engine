#include "latebit/core/graphics/Colors.h"

#include <array>

#include "test/lib/test.h"

using namespace std;
using namespace lb;

auto main() -> int {
  test("converts hex to colors", []() {
    array<char, 22> validColors = {'0', '1', '2', '3', '4', '5', '6', '7',
                                   '8', '9', 'A', 'a', 'B', 'b', 'C', 'c',
                                   'D', 'd', 'E', 'e', 'F', 'f'};
    array<Color::Color, 22> expectedColors = {
      Color::BLACK,  Color::DARK_BLUE, Color::DARK_PURPLE, Color::DARK_GREEN,
      Color::BROWN,  Color::DARK_GRAY, Color::LIGHT_GRAY,  Color::WHITE,
      Color::RED,    Color::ORANGE,    Color::YELLOW,      Color::YELLOW,
      Color::GREEN,  Color::GREEN,     Color::BLUE,        Color::BLUE,
      Color::INDIGO, Color::INDIGO,    Color::PINK,        Color::PINK,
      Color::PEACH,  Color::PEACH};

    for (int i = 0; i < 22; i++) {
      Color::Color expectedColor = expectedColors[i];
      Color::Color actualColor = fromHex(validColors[i]);
      auto msg = "returns correct color for " + string(1, validColors[i]);
      assertEq(msg, actualColor, expectedColor);
    }

    array<char, 2> invalidColors = {'$', 'K'};
    Color::Color undefinedColor = Color::UNDEFINED_COLOR;
    for (auto& invalidColor : invalidColors) {
      Color::Color actualColor = fromHex(invalidColor);
      assertEq("returns UNDEFINED_COLOR for invalid string", actualColor,
               undefinedColor);
    }
  });

  test("converts colors to hex", []() {
    array<Color::Color, 16> validColors = {
      Color::BLACK, Color::DARK_BLUE, Color::DARK_PURPLE, Color::DARK_GREEN,
      Color::BROWN, Color::DARK_GRAY, Color::LIGHT_GRAY,  Color::WHITE,
      Color::RED,   Color::ORANGE,    Color::YELLOW,      Color::GREEN,
      Color::BLUE,  Color::INDIGO,    Color::PINK,        Color::PEACH};
    array<char, 16> expectedHex = {'0', '1', '2', '3', '4', '5', '6', '7',
                                   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 0; i < 15; i++) {
      char hex = toHex(validColors[i]);
      auto msg = "returns correct color for " + string(1, hex);
      assertEq(msg, expectedHex[i], hex);
    }
  });

  return report();
}