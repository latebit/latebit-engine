#include "core/geometry/Vector.h"

#include <cmath>

#include "../../../test/lib/test.h"

void Vector_getMagnitude_test() {
  Vector null;
  Vector unary(1);
  Vector another(1, 1);

  assertEq("has null magnitude", null.getMagnitude(), 0.0);
  assertEq("has unary magnitude", unary.getMagnitude(), 1.0);
  assertEq("has correct magnitude", another.getMagnitude(), std::sqrt(2));
};

void Vector_scale_test() {
  Vector subject;
  subject.scale(10);
  assertEq("scales nothing", subject.getMagnitude(), 0.0);

  subject = Vector(1, 1);
  subject.scale(2);

  assertEq("scales correctly", subject.getMagnitude(), std::sqrt(8));
}

void Vector_distance_test() {
  Vector a;
  Vector b(1, 0);
  assertEq("vertical distance", a.distance(&b), 1.0);
  b = Vector(0, 1);
  assertEq("horizontal distance", a.distance(&b), 1.0);
  b = Vector(1, 1);
  assertEq("other distance", a.distance(&b), std::sqrt(2));
}

void Vector_normalize_test() {
  Vector subject;
  subject.normalize();

  assertEq("does nothing with null length", subject.getMagnitude(), 0.0);

  subject = Vector(123, 345);
  subject.normalize();
  assertEq("has length 1", subject.getMagnitude(), 1.0);
}

void Vector_eq_test() {
  Vector a, b;
  assert("compares same vector", a == b, "wanted true got false");

  b = Vector(1, 2);
  assert("compares different vectors", !(a == b), "wanted true got false");
}

void Vector_plus_test() {
  Vector a, b, got, want;

  got = a + b;
  assertEq("sums zero vectors", got, want);

  b = Vector(1, 1);
  got = a + b;
  want = Vector(1, 1);
  assertEq("handles zero vector", got, want);

  a = Vector(1, 2);
  got = a + b;
  want = Vector(2, 3);
  assertEq("handles any other vector", got, want);
}

void Vector_not_test() {
  Vector subject;
  assert("zero vector", !subject);

  subject = Vector(1, 1);
  assert("non-zero vector", !!subject);
}

void Vector_test() {
  test("getMagnitude", Vector_getMagnitude_test);
  test("scale", Vector_scale_test);
  test("distance", Vector_distance_test);
  test("normalize", Vector_normalize_test);
  test("equals (==)", Vector_eq_test);
  test("plus (+)", Vector_plus_test);
  test("not (!)", Vector_not_test);
}