#pragma once

#include "Event.h"
namespace df {
const std::string OUT_EVENT = "df::out";

class EventOut : public Event {
 public:
  // Creates an event signaling an object has gone out of bounds
  EventOut();
};

}  // namespace df
