#include "EventStep.h"

namespace df {
EventStep::EventStep() {
  m_step_count = 0;
  setType(STEP_EVENT);
}

EventStep::EventStep(int count) {
  EventStep();
  setStepCount(count);
}

void EventStep::setStepCount(int count) { m_step_count = count; }

int EventStep::getStepCount() { return m_step_count; }
}  // namespace df
