#include "../include/Colors.h"

#include "test.h"

auto Colors_fromColorString_test() -> int {
  int result = 0;

  printf("fromColorString_test\n");
  // Test valid color strings
  std::string validColors[] = {"black", "blue",    "red",  "yellow",
                               "green", "magenta", "cyan", "white"};
  df::Color expectedColors[] = {df::BLACK, df::BLUE,    df::RED,  df::YELLOW,
                                df::GREEN, df::MAGENTA, df::CYAN, df::WHITE};
  for (int i = 0; i < 8; i++) {
    df::Color expectedColor = expectedColors[i];
    df::Color actualColor = df::fromColorString(validColors[i]);
    result += assert_int("returns correct color for valid string", actualColor,
                         expectedColor);
  }

  // Test invalid color strings
  std::string invalidColors[] = {"invalid", "123", "orange", "purple"};
  df::Color undefinedColor = df::UNDEFINED_COLOR;
  for (auto& invalidColor : invalidColors) {
    df::Color actualColor = df::fromColorString(invalidColor);
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