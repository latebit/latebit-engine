#include "latebit/core/events/EventStep.h"

#include "../../../test/lib/test.h"

using namespace lb;

auto main() -> int {
  EventStep subject;
  assertEq("is initialised with type", subject.getType(), STEP_EVENT);

  EventStep subject2 = EventStep(10);
  assertEq("is initialised with count", subject2.getStepCount(), 10);

  return report();
}