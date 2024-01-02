#include "colors.h"

#include <iostream>

bool supportsEscapeSequences =
    (std::cout << "\033[31mTest\033[0m", std::cout.flush(), true);

std::string color(std::string color, std::string s) {
  if (supportsEscapeSequences) {
    return "\033[" + color + "m" + s + "\033[0m";
  }

  return s;
}

std::string red(std::string s) { return color("31", s); }
std::string green(std::string s) { return color("32", s); }
std::string yellow(std::string s) { return color("33", s); }