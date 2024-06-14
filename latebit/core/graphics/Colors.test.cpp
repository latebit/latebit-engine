#include "latebit/core/graphics/Colors.h"

#include <array>

#include "../../../test/lib/test.h"

using namespace std;
using namespace lb;

auto main() -> int {
  test("converts hex to colors", []() {
    array<char, 22> validColors = {'0', '1', '2', '3', '4', '5', '6', '7',
                                   '8', '9', 'A', 'a', 'B', 'b', 'C', 'c',
                                   'D', 'd', 'E', 'e', 'F', 'f'};
    array<Color, 22> expectedColors = {
      BLACK,      DARK_BLUE, DARK_PURPLE, DARK_GREEN, BROWN,  DARK_GRAY,
      LIGHT_GRAY, WHITE,     RED,         ORANGE,     YELLOW, YELLOW,
      GREEN,      GREEN,     BLUE,        BLUE,       INDIGO, INDIGO,
      PINK,       PINK,      PEACH,       PEACH};

    for (int i = 0; i < 22; i++) {
      Color expectedColor = expectedColors[i];
      Color actualColor = fromHex(validColors[i]);
      auto msg = "returns correct color for " + string(1, validColors[i]);
      assertEq(msg, actualColor, expectedColor);
    }

    array<char, 2> invalidColors = {'$', 'K'};
    Color undefinedColor = UNDEFINED_COLOR;
    for (auto& invalidColor : invalidColors) {
      Color actualColor = fromHex(invalidColor);
      assertEq("returns UNDEFINED_COLOR for invalid string", actualColor,
               undefinedColor);
    }
  });

  test("converts colors to hex", []() {
    array<Color, 16> validColors = {BLACK, DARK_BLUE, DARK_PURPLE, DARK_GREEN,
                                    BROWN, DARK_GRAY, LIGHT_GRAY,  WHITE,
                                    RED,   ORANGE,    YELLOW,      GREEN,

                                    BLUE,  INDIGO,    PINK,        PEACH};
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