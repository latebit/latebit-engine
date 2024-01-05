#include "../includes/utils.h"

#include "test.h"

int utils_positionsMatch_test() {
  int result = 0;

  df::Vector v1(1, 2);
  df::Vector v2(1, 2);
  df::Vector v3(3, 1);
  df::Vector v4(1.5, 1.5);

  result += assert("returns true if same position", positionsMatch(v1, v2));
  result += assert("returns true if close positions", positionsMatch(v1, v4));
  result +=
      assert("returns false if positions are too far", !positionsMatch(v1, v3));

  return result;
}

int utils_test() {
  int result = 0;

  result += assert_regex("returns date in correct format", getTimeString(),
                         "[0-9]{2}:[0-9]{2}:[0-9]{2}");

  result += utils_positionsMatch_test();

  return result;
}