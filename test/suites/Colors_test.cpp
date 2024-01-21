#include "Colors.h"

#include <array>

#include "../lib/test.h"

using namespace std;
using namespace df;

void Colors_fromColorString_test() {
  array<string, 8> validColors = {"black", "blue",    "red",  "yellow",
                                  "green", "magenta", "cyan", "white"};
  array<Color, 8> expectedColors = {BLACK, BLUE,    RED,  YELLOW,
                                    GREEN, MAGENTA, CYAN, WHITE};
  for (int i = 0; i < 8; i++) {
    Color expectedColor = expectedColors[i];
    Color actualColor = fromColorString(validColors[i]);
    assert_int("returns correct color for " + validColors[i], actualColor,
               expectedColor);
  }

  array<string, 4> invalidColors = {"invalid", "123", "orange", "purple"};
  Color undefinedColor = UNDEFINED_COLOR;
  for (auto& invalidColor : invalidColors) {
    Color actualColor = fromColorString(invalidColor);
    assert_int("returns UNDEFINED_COLOR for invalid string", actualColor,
               undefinedColor);
  }
}

void Colors_test() { test("fromColorString", Colors_fromColorString_test); }