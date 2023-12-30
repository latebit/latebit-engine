#include <iostream>

#include "Object_test.h"
#include "Vector_test.h"
#include "utils_test.h"

int main() {
  int result = 0;
  std::cout << "[[[ utils_test ]]]" << std::endl;
  result += utils_test();
  std::cout << "[[[ Vector_test ]]]" << std::endl;
  result += Vector_test();
  std::cout << "[[[ Object_test ]]]" << std::endl;
  result += Object_test();

  if (result == 0) {
    std::cout << "Success! All tests passed successfully" << std::endl;
  } else {
    printf("Failure. %d failed tests.\n", result);
  }

  return result;
};