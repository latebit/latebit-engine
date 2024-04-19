#include "core/geometry/Box.h"

#include "../../../test/lib/test.h"
#include "core/geometry/Vector.h"

void constructor() {
  Box box;
  assertEq("default constructor sets top left corner to (0, 0)",
           box.getCorner(), Vector());

  assertEq("default constructor sets width to 0", box.getWidth(), 0.0);

  assertEq("default constructor sets height to 0", box.getHeight(), 0.0);
}

void parametrizedConstructor() {
  Vector toleft(1, 2);
  float width = 3.5;
  float height = 4.5;
  Box box = Box(toleft, width, height);

  assertEq("parameterized constructor sets top left corner correctly",
           box.getCorner(), toleft);
  assertEq("parameterized constructor sets width correctly", box.getWidth(),
           width);
  assertEq("parameterized constructor sets height correctly", box.getHeight(),
           height);
}

void setCorner() {
  Box box;
  Vector corner(5, 6);
  box.setCorner(corner);
  assertEq("sets top left corner correctly", box.getCorner(), corner);
}

void setWidth() {
  Box box;
  float width = 10.5;
  box.setWidth(width);
  assertEq("sets width correctly", box.getWidth(), width);
}

void setHeight() {
  Box box;
  float height = 15.5;
  box.setHeight(height);
  assertEq("sets height correctly", box.getHeight(), height);
}

auto main() -> int {
  test("constructor", constructor);
  test("parametrized constructor", parametrizedConstructor);
  test("setCorner", setCorner);
  test("setWidth", setWidth);
  test("setHeight", setHeight);
  return report();
}