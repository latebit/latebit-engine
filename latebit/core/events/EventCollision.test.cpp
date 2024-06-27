#include "latebit/core/events/EventCollision.h"

#include "test/lib/test.h"

using namespace lb;

auto main() -> int {
  EventCollision subject;
  assert("sets default object1", subject.getFirstObject() == nullptr);
  assert("sets default object2", subject.getSecondObject() == nullptr);
  assertEq("sets default position", subject.getPosition(), Vector());
  assertEq("sets type", subject.getType(), COLLISION_EVENT);

  Object obj1, obj2;
  Vector pos(1, 1);
  auto withParams = EventCollision(&obj1, &obj2, pos);
  assert("sets correct object1", withParams.getFirstObject() == &obj1);
  assert("sets correct object2", withParams.getSecondObject() == &obj2);
  assertEq("sets correct position", withParams.getPosition(), pos);
  assertEq("sets type", withParams.getType(), COLLISION_EVENT);

  return report();
}