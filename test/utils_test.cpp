#include "../src/utils.h"

#include "test.h"

int utils_test() {
  int result = 0;

  result += assert_regex("returns date in correct format", getTimeString(),
                         "[0-9]{2}:[0-9]{2}:[0-9]{2}");

  return result;
}