#include "EventStep.h"

namespace df {
EventStep::EventStep() {
  this->step_count = 0;
  setType(STEP_EVENT);
}

EventStep::EventStep(int count) {
  this->step_count = 0;
  setType(STEP_EVENT);
  this->step_count = count;
}

void EventStep::setStepCount(int count) { this->step_count = count; }

int EventStep::getStepCount() const { return this->step_count; }
}  // namespace df
