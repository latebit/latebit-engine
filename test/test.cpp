#include "test.h"

#include <iostream>
#include <regex>
#include <sstream>

#include "../include/Vector.h"
#include "../include/utils.h"
#include "colors.h"
#include "test.h"

int ASSERTIONS = 0;
auto getAssertions() -> std::string { return std::to_string(ASSERTIONS); }

bool FAILED_ONLY =
    std::getenv("FAILED_ONLY") && std::stoi(std::getenv("FAILED_ONLY")) == 1;

auto assert(const std::string name, bool assertion, const std::string message)
    -> int {
  ASSERTIONS++;

  if (!assertion) {
    std::cout << red("FAIL: " + name) << std::endl;
    std::cout << "      " << message << std::endl;
    return 1;
  }

  if (!FAILED_ONLY) std::cout << green("PASS: " + name) << std::endl;
  return 0;
}

auto assert_string(std::string name, std::string got, std::string want) -> int {
  const bool assertion = got.compare(want) == 0;

  std::string message = "wanted '" + want + "' got '" + got + "'";

  return assert(name, assertion, message);
}

auto assert_regex(std::string name, std::string want, std::string pattern)
    -> int {
  std::regex expression(pattern);
  const bool assertion = std::regex_match(want, expression);

  std::string message =
      "string '" + want + "' doesn't match pattern  '" + pattern + "'";

  return assert(name, assertion, message);
}

auto assert_float(std::string name, float got, float want) -> int {
  std::string message =
      "wanted '" + std::to_string(want) + "' got '" + std::to_string(got) + "'";

  return assert(name, equals(got, want), message);
}

auto assert_int(std::string name, int got, int want) -> int {
  std::string message =
      "wanted '" + std::to_string(want) + "' got '" + std::to_string(got) + "'";

  return assert(name, got == want, message);
}

auto assert_ok(std::string name, int got) -> int {
  return assert_int(name, got, 0);
}
auto assert_fail(std::string name, int got) -> int {
  return assert_int(name, got, -1);
}

auto assert_vector(std::string name, df::Vector got, df::Vector want) -> int {
  return assert(name, got == want,
                "wanted " + want.toString() + " got " + got.toString());
}