#include <iostream>

#include "colors.h"
#include "test.h"

auto run(void (*tests)()) -> int {
  auto c = getClock();

  auto initial = c.getNow();
  tests();
  auto final = c.getNow();

  auto failed = getFailedAssertions();
  auto total = getAssertions();

  if (failed == 0) {
    std::cout << std::endl
              << green("Success! All " + total + " assertions passed. ");
    printf("Total Duration: %.2fms\n",
           static_cast<double>(final - initial) / 1000.0);
  } else {
    std::cout << std::endl
              << red("Failure. " + std::to_string(failed) + " failed tests.")
              << std::endl;
  }

  return failed;
};
