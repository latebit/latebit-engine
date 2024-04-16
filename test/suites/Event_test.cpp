#include "core/events/Event.h"

#include "../lib/test.h"

using namespace lb;

void Event_test() {
  Event subject;

  subject.setType("type");
  assertEq("updates type", subject.getType(), "type");
}