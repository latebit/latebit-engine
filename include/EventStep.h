#pragma once

#include "Event.h"

namespace df {
const std::string STEP_EVENT = "df::step";

class EventStep : public Event {
 private:
  int m_step_count;

 public:
  EventStep();
  EventStep(int count);

  void setStepCount(int count);
  int getStepCount() const;
};
}  // namespace df

