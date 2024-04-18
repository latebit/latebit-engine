#include "core/utils/utils.h"

#include "../lib/test.h"

void utils_match_test() {
  Vector v1(1, 2);
  Vector v2(1, 2);
  Vector v3(3, 1);
  Vector v4(1.5, 1.5);

  assert("returns true if same position", match(v1, v2));
  assert("returns true if close positions", match(v1, v4));
  assert("returns false if positions are too far", !match(v1, v3));
}

void utils_intersects_test() {
  Box intersecting(Vector(-1, 0), 2, 2);
  Box nonIntersecting(Vector(3, 3), 2, 2);
  Box subject(Vector(0, 0), 2, 2);

  assert("returns true if boxes intersect (left)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(1, 0));
  assert("returns true if boxes intersect (right)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(0, -1));
  assert("returns true if boxes intersect (top)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(0, 1));
  assert("returns true if boxes intersect (bottom)",
         intersects(subject, intersecting));

  intersecting.setCorner(Vector(0, 0));
  intersecting.setWidth(1);
  intersecting.setHeight(1);
  assert("returns true if boxes intersect (inside)",
         intersects(subject, intersecting));

  intersecting.setWidth(3);
  intersecting.setHeight(3);
  assert("returns true if boxes intersect (outside)",
         intersects(subject, intersecting));

  intersecting.setWidth(2);
  intersecting.setHeight(2);

  intersecting.setCorner(Vector(-2, 0));
  assert("returns true if boxes have same border (left)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(2, 0));
  assert("returns true if boxes have same border (right)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(0, -2));
  assert("returns true if boxes have same border (top)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(0, 2));
  assert("returns true if boxes have same border (bottom)",
         intersects(subject, intersecting));

  intersecting.setCorner(Vector(-2, -2));
  assert("returns true if boxes have same corner (top left)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(2, -2));
  assert("returns true if boxes have same corner (top right)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(-2, 2));
  assert("returns true if boxes have same corner (bottom left)",
         intersects(subject, intersecting));
  intersecting.setCorner(Vector(2, 2));
  assert("returns true if boxes have same corner (bottom right)",
         intersects(subject, intersecting));

  assert("returns false if boxes do not intersect",
         !intersects(subject, nonIntersecting));
}

auto utils_test() -> void {
  test("match", utils_match_test);
  test("intersects", utils_intersects_test);
}