#include "../includes/utils.h"

#include "test.h"

int utils_match_test() {
  int result = 0;

  df::Vector v1(1, 2);
  df::Vector v2(1, 2);
  df::Vector v3(3, 1);
  df::Vector v4(1.5, 1.5);

  result += assert("returns true if same position", match(v1, v2));
  result += assert("returns true if close positions", match(v1, v4));
  result += assert("returns false if positions are too far", !match(v1, v3));

  return result;
}

int utils_intersects_test() {
  int result = 0;

  df::Box subject(df::Vector(0, 0), 2, 2);
  df::Box intersecting(df::Vector(-1, 0), 2, 2);
  df::Box nonIntersecting(df::Vector(3, 3), 2, 2);

  result += assert("returns true if boxes intersect (left)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(1, 0));
  result += assert("returns true if boxes intersect (right)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(0, -1));
  result += assert("returns true if boxes intersect (top)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(0, 1));
  result += assert("returns true if boxes intersect (bottom)",
                   intersects(subject, intersecting));

  intersecting.setCorner(df::Vector(0, 0));
  intersecting.setWidth(1);
  intersecting.setHeight(1);
  result += assert("returns true if boxes intersect (inside)",
                   intersects(subject, intersecting));

  intersecting.setWidth(3);
  intersecting.setHeight(3);
  result += assert("returns true if boxes intersect (outside)",
                   intersects(subject, intersecting));

  intersecting.setWidth(2);
  intersecting.setHeight(2);

  intersecting.setCorner(df::Vector(-2, 0));
  result += assert("returns true if boxes have same border (left)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(2, 0));
  result += assert("returns true if boxes have same border (right)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(0, -2));
  result += assert("returns true if boxes have same border (top)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(0, 2));
  result += assert("returns true if boxes have same border (bottom)",
                   intersects(subject, intersecting));

  intersecting.setCorner(df::Vector(-2, -2));
  result += assert("returns true if boxes have same corner (top left)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(2, -2));
  result += assert("returns true if boxes have same corner (top right)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(-2, 2));
  result += assert("returns true if boxes have same corner (bottom left)",
                   intersects(subject, intersecting));
  intersecting.setCorner(df::Vector(2, 2));
  result += assert("returns true if boxes have same corner (bottom right)",
                   intersects(subject, intersecting));

  result += assert("returns false if boxes do not intersect",
                   !intersects(subject, nonIntersecting));

  return result;
}

int utils_test() {
  int result = 0;

  result += assert_regex("returns date in correct format", getTimeString(),
                         "[0-9]{2}:[0-9]{2}:[0-9]{2}");

  result += utils_match_test();
  result += utils_intersects_test();

  return result;
}