#include "EventStep.h"

namespace df {
EventStep::EventStep() {
  setType(STEP_EVENT);
  this->stepCount = 0;
}

EventStep::EventStep(int count) {
  setType(STEP_EVENT);
  this->stepCount = count;
}

void EventStep::setStepCount(int c) { this->stepCount = c; }
int EventStep::getStepCount() const { return this->stepCount; }
}  // namespace df
