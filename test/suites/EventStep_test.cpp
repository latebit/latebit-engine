#include "core/events/EventStep.h"

#include "../lib/test.h"

using namespace lb;

void EventStep_test() {
  EventStep subject;
  assertEq("is initialised with type", subject.getType(), STEP_EVENT);

  subject = EventStep(10);
  assertEq("is initialised with count", subject.getStepCount(), 10);

  subject.setStepCount(2);
  assertEq("has correct count", subject.getStepCount(), 2);
}