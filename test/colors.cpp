#include "colors.h"

#include <iostream>

bool supportsEscapeSequences =
    (std::cout << "\033[31m\033[0m", std::cout.flush(), true);

auto color(std::string color, std::string s) -> std::string {
  if (supportsEscapeSequences) {
    return "\033[" + color + "m" + s + "\033[0m";
  }

  return s;
}

auto red(std::string s) -> std::string { return color("31", s); }
auto green(std::string s) -> std::string { return color("32", s); }
auto yellow(std::string s) -> std::string { return color("33", s); }