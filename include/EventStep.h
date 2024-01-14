#pragma once

#include "Event.h"

namespace df {
const std::string STEP_EVENT = "df::step";

class EventStep : public Event {
 private:
  int stepCount = 0;

 public:
  EventStep();
  EventStep(int count);

  void setStepCount(int count);
  auto getStepCount() const -> int;
};
}  // namespace df
