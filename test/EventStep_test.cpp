#include "../include/EventStep.h"

#include "test.h"

auto EventStep_test() -> int {
  int result = 0;

  auto *subject = new df::EventStep;
  result += assert_string("is initialised with type", subject->getType(),
                          df::STEP_EVENT);

  subject = new df::EventStep(10);
  result +=
      assert_int("is initialised with count", subject->getStepCount(), 10);

  subject->setStepCount(2);
  result += assert_int("has correct count", subject->getStepCount(), 2);

  return result;
}