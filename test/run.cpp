#include <iostream>

#include "utils_test.h"

int main() {
  int result = 0;
  result += utils_test();

  if (result == 0) {
    std::cout << "Success! All tests passed successfully" << std::endl;
  } else {
    printf("Failure. %d failed tests.\n", result);
  }

  return result;
};