#pragma once

#include "Event.h"

namespace lb {
const std::string STEP_EVENT = "lb::step";

class EventStep : public Event {
 private:
  const int stepCount = 0;

 public:
  // Creates a new event on every step of the game loop
  EventStep(int count = 0) : Event(STEP_EVENT), stepCount(count) {};

  // Returns the step count
  [[nodiscard]] auto getStepCount() const -> int;
};
}  // namespace lb
