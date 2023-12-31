#ifndef __EVENT_STEP_H__
#define __EVENT_STEP_H__

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
  int getStepCount();
};
}  // namespace df

#endif