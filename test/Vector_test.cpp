#include "../src/Vector.h"

#include <cmath>
#include <iostream>

#include "test.h"

int assert_vector(std::string name, df::Vector *got, df::Vector *want) {
  return assert(name, *got == *want,
                "wanted " + want->toString() + " got " + got->toString());
}

int Vector_getMagnitude_test() {
  df::Vector *p_null = new df::Vector();
  df::Vector *p_unary = new df::Vector(1);
  df::Vector *p_another = new df::Vector(1, 1);

  int result = 0;
  result += assert_float("has null magnitude", p_null->getMagnitude(), 0);
  result += assert_float("has unary magnitude", p_unary->getMagnitude(), 1);
  result += assert_float("has correct magnitude", p_another->getMagnitude(),
                         std::sqrt(2));

  return result;
};

int Vector_scale_test() {
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

int Vector_distance_test() {
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

int Vector_normalise_test() {
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

int Vector_eq_test() {
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

int Vector_plus_test() {
  df::Vector *a, *b, got, *want;

  int result = 0;

  a = new df::Vector;
  b = new df::Vector;
  got = *a + *b;
  want = new df::Vector;
  result += assert_vector("sums zero vectors", &got, want);

  b = new df::Vector(1, 1);
  got = *a + *b;
  want = new df::Vector(1, 1);
  result += assert_vector("handles zero vector", &got, want);

  a = new df::Vector(1, 2);
  got = *a + *b;
  want = new df::Vector(2, 3);
  result += assert_vector("handles any other vector", &got, want);

  return result;
}

int Vector_not_test() {
  df::Vector *subject;

  int result = 0;
  subject = new df::Vector;
  result += assert("zero vector", !*subject);

  subject = new df::Vector(1, 1);
  result += assert("non-zero vector", !!*subject);

  return result;
}

int Vector_test() {
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