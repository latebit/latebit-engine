#include "Event.h"

namespace df {
Event::Event() { this->event_type = UNDEFINED_EVENT; }

Event::~Event() = default;

void Event::setType(std::string t) { this->event_type = t; }

auto Event::getType() const -> std::string { return this->event_type; }
}  // namespace df
