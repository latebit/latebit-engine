#include "Event.h"

namespace df {
Event::Event() { this->event_type = UNDEFINED_EVENT; }

Event::~Event() {}

void Event::setType(std::string t) { this->event_type = t; }

std::string Event::getType() const { return this->event_type; }
}  // namespace df
