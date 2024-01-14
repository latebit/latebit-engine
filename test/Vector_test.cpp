#include "../include/Vector.h"

#include <cmath>
#include <iostream>

#include "test.h"

auto Vector_getMagnitude_test() -> int {
  auto *null = new df::Vector();
  auto *unary = new df::Vector(1);
  auto *another = new df::Vector(1, 1);

  int result = 0;
  result += assert_float("has null magnitude", null->getMagnitude(), 0);
  result += assert_float("has unary magnitude", unary->getMagnitude(), 1);
  result += assert_float("has correct magnitude", another->getMagnitude(),
                         std::sqrt(2));

  return result;
};

auto Vector_scale_test() -> int {
  df::Vector *subject;

  int result = 0;

  subject = new df::Vector();
  subject->scale(10);
  result += assert_float("scales nothing", subject->getMagnitude(), 0);

  subject = new df::Vector(1, 1);
  subject->scale(2);
  result +=
      assert_float("scales correctly", subject->getMagnitude(), std::sqrt(8));

  return result;
}

auto Vector_distance_test() -> int {
  df::Vector *a, *b;

  int result = 0;

  a = new df::Vector();
  b = new df::Vector(1, 0);
  result += assert_float("vertical distance", a->distance(b), 1);
  b = new df::Vector(0, 1);
  result += assert_float("horizontal distance", a->distance(b), 1);
  b = new df::Vector(1, 1);
  result += assert_float("other distance", a->distance(b), std::sqrt(2));

  return result;
}

auto Vector_normalise_test() -> int {
  df::Vector *subject;

  int result = 0;

  subject = new df::Vector();
  subject->normalize();
  result +=
      assert_float("does nothing with null length", subject->getMagnitude(), 0);

  subject = new df::Vector(123, 345);
  subject->normalize();
  result += assert_float("has length 1", subject->getMagnitude(), 1);

  return result;
}

auto Vector_eq_test() -> int {
  df::Vector *a, *b;

  int result = 0;

  a = new df::Vector;
  b = new df::Vector;
  result += assert("compares same vector", *a == *b, "wanted true got false");
  b = new df::Vector(1, 2);
  result += assert("compares different vectors", !(*a == *b),
                   "wanted true got false");

  return result;
}

auto Vector_plus_test() -> int {
  df::Vector *a, *b, got, want;

  int result = 0;

  a = new df::Vector;
  b = new df::Vector;
  got = *a + *b;
  want = df::Vector();
  result += assert_vector("sums zero vectors", got, want);

  b = new df::Vector(1, 1);
  got = *a + *b;
  want = df::Vector(1, 1);
  result += assert_vector("handles zero vector", got, want);

  a = new df::Vector(1, 2);
  got = *a + *b;
  want = df::Vector(2, 3);
  result += assert_vector("handles any other vector", got, want);

  return result;
}

auto Vector_not_test() -> int {
  df::Vector *subject;

  int result = 0;
  subject = new df::Vector;
  result += assert("zero vector", !*subject);

  subject = new df::Vector(1, 1);
  result += assert("non-zero vector", !!*subject);

  return result;
}

auto Vector_test() -> int {
  int result = 0;
  std::cout << "Vector_getMagnitude_test" << std::endl;
  result += Vector_getMagnitude_test();
  std::cout << "Vector_scale_test" << std::endl;
  result += Vector_scale_test();
  std::cout << "Vector_distance_test" << std::endl;
  result += Vector_distance_test();
  std::cout << "Vector_normalize_test" << std::endl;
  result += Vector_normalise_test();
  std::cout << "Vector_eq_test" << std::endl;
  result += Vector_eq_test();
  std::cout << "Vector_plus_test" << std::endl;
  result += Vector_plus_test();
  std::cout << "Vector_not_test" << std::endl;
  result += Vector_not_test();

  return result;
}