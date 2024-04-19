#include "core/geometry/Vector.h"

#include <cmath>

#include "../../../test/lib/test.h"

void getMagnitude() {
  Vector null;
  Vector unary(1);
  Vector another(1, 1);

  assertEq("has null magnitude", null.getMagnitude(), 0.0);
  assertEq("has unary magnitude", unary.getMagnitude(), 1.0);
  assertEq("has correct magnitude", another.getMagnitude(), std::sqrt(2));
};

void scale() {
  Vector subject;
  subject.scale(10);
  assertEq("scales nothing", subject.getMagnitude(), 0.0);

  subject = Vector(1, 1);
  subject.scale(2);

  assertEq("scales correctly", subject.getMagnitude(), std::sqrt(8));
}

void distance() {
  Vector a;
  Vector b(1, 0);
  assertEq("vertical distance", a.distance(&b), 1.0);
  b = Vector(0, 1);
  assertEq("horizontal distance", a.distance(&b), 1.0);
  b = Vector(1, 1);
  assertEq("other distance", a.distance(&b), std::sqrt(2));
}

void normalize() {
  Vector subject;
  subject.normalize();

  assertEq("does nothing with null length", subject.getMagnitude(), 0.0);

  subject = Vector(123, 345);
  subject.normalize();
  assertEq("has length 1", subject.getMagnitude(), 1.0);
}

void eq() {
  Vector a, b;
  assert("compares same vector", a == b, "wanted true got false");

  b = Vector(1, 2);
  assert("compares different vectors", !(a == b), "wanted true got false");
}

void plusOperator() {
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

void notOperator() {
  Vector subject;
  assert("zero vector", !subject);

  subject = Vector(1, 1);
  assert("non-zero vector", !!subject);
}

auto main() -> int {
  test("getMagnitude", getMagnitude);
  test("scale", scale);
  test("distance", distance);
  test("normalize", normalize);
  test("equals (==)", eq);
  test("plus (+)", plusOperator);
  test("not (!)", notOperator);
  return report();
}