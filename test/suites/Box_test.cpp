#include "Box.h"

#include "../lib/test.h"
#include "Vector.h"

void constructor_test() {
  Box box;
  int result =
    assert_vector("default constructor sets top left corner to (0, 0)",
                  box.getCorner(), Vector());

  assert_float("default constructor sets width to 0", box.getWidth(), 0);

  assert_float("default constructor sets height to 0", box.getHeight(), 0);

  Vector toleft(1, 2);
  float width = 3.5;
  float height = 4.5;
  box = Box(toleft, width, height);

  assert_vector("parameterized constructor sets top left corner correctly",
                box.getCorner(), toleft);
  assert_float("parameterized constructor sets width correctly", box.getWidth(),
               width);
  assert_float("parameterized constructor sets height correctly",
               box.getHeight(), height);
}

void setCorner_test() {
  Box box;
  Vector corner(5, 6);
  box.setCorner(corner);
  assert_vector("sets top left corner correctly", box.getCorner(), corner);
}

void setWidth_test() {
  Box box;
  float width = 10.5;
  box.setWidth(width);
  assert_float("sets width correctly", box.getWidth(), width);
}

void setHeight_test() {
  Box box;
  float height = 15.5;
  box.setHeight(height);
  assert_float("sets height correctly", box.getHeight(), height);
}

void Box_test() {
  test("constructor", constructor_test);
  test("setCorner", setCorner_test);
  test("setWidth", setWidth_test);
  test("setHeight", setHeight_test);
}