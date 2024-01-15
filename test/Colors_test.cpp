#include "../include/Colors.h"

#include <array>

#include "test.h"

using namespace std;
using namespace df;

auto Colors_fromColorString_test() -> int {
  int result = 0;

  printf("fromColorString_test\n");
  // Test valid color strings
  array<string, 8> validColors = {"black", "blue",    "red",  "yellow",
                                  "green", "magenta", "cyan", "white"};
  array<Color, 8> expectedColors = {BLACK, BLUE,    RED,  YELLOW,
                                    GREEN, MAGENTA, CYAN, WHITE};
  for (int i = 0; i < 8; i++) {
    Color expectedColor = expectedColors[i];
    Color actualColor = fromColorString(validColors[i]);
    result += assert_int("returns correct color for valid string", actualColor,
                         expectedColor);
  }

  // Test invalid color strings
  array<string, 4> invalidColors = {"invalid", "123", "orange", "purple"};
  Color undefinedColor = UNDEFINED_COLOR;
  for (auto& invalidColor : invalidColors) {
    Color actualColor = fromColorString(invalidColor);
    result += assert_int("returns UNDEFINED_COLOR for invalid string",
                         actualColor, undefinedColor);
  }

  return result;
}

auto Colors_test() -> int {
  int result = 0;

  result += Colors_fromColorString_test();

  return result;
}