#pragma once

#include "Event.h"
namespace df {
const std::string OUT_EVENT = "df::out";

class EventOut : public Event {
 public:
  EventOut();
};

}  // namespace df
