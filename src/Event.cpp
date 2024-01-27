#include "Event.h"

namespace df {
Event::Event() = default;
Event::~Event() = default;

void Event::setType(std::string t) { this->type = t; }

auto Event::getType() const -> std::string { return this->type; }
}  // namespace df
