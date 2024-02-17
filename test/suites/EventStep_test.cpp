#include "EventStep.h"

#include "../lib/test.h"

using namespace lb;

void EventStep_test() {
  EventStep subject;
  assert_string("is initialised with type", subject.getType(), STEP_EVENT);

  subject = EventStep(10);
  assert_int("is initialised with count", subject.getStepCount(), 10);

  subject.setStepCount(2);
  assert_int("has correct count", subject.getStepCount(), 2);
}