#include "Event.h"

namespace df {
Event::Event() { m_event_type = UNDEFINED_EVENT; }

Event::~Event() {}

void Event::setType(std::string t) { m_event_type = t; }

std::string Event::getType() const { return m_event_type; }
}  // namespace df
