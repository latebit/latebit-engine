#pragma once

#include "Event.h"
namespace lb {
const std::string OUT_EVENT = "lb::out";

class EventOut : public Event {
 public:
  // Creates an event signaling an object has gone out of bounds
  EventOut();
};

}  // namespace lb
