#include "latebit/core/events/EventOverlap.h"

#include "test/lib/test.h"

using namespace lb;

auto main() -> int {
  Object obj1, obj2;
  Vector pos(1, 1);
  auto withParams = EventOverlap(&obj1, &obj2, pos);
  assert("sets correct object1", withParams.getFirstObject() == &obj1);
  assert("sets correct object2", withParams.getSecondObject() == &obj2);
  assertEq("sets correct position", withParams.getPosition(), pos);
  assertEq("sets type", withParams.getType(), "lb::overlap");

  return report();
}