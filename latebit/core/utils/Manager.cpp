#include "Manager.h"

#include "latebit/core/events/Event.h"
#include "latebit/core/objects/ObjectUtils.h"

namespace lb {

auto Manager::getType() const -> std::string { return this->type; }

auto Manager::isStarted() const -> bool { return this->started; }

auto Manager::startUp() -> int {
  this->started = true;
  return 0;
}

void Manager::shutDown() { this->started = false; }

auto Manager::subscribe(Object* o, string eventType) -> int {
  if (!this->isValid(eventType)) {
    return -1;
  }

  if (this->eventCount >= MAX_EVENTS) return -1;

  for (int i = 0; i < this->eventCount; i++) {
    if (this->events[i] == eventType) {
      this->subscribers[i].push_back(o);
      return 0;
    }
  }

  // Subscribe to a new event
  this->events[this->eventCount] = eventType;
  this->subscribers[this->eventCount].clear();
  this->subscribers[this->eventCount].push_back(o);
  this->eventCount++;

  return 0;
}

auto Manager::unsubscribe(Object* o, string eventType) -> int {
  if (!this->isValid(eventType)) {
    return -1;
  }

  int foundIndex = 0;
  for (int i = 0; i < this->eventCount; i++) {
    if (this->events[i] == eventType) {
      remove(this->subscribers[i], o);
      foundIndex = i;
      break;
    }
  }

  if (this->subscribers[foundIndex].empty()) {
    this->subscribers[foundIndex] = this->subscribers[this->eventCount];
    this->events[foundIndex] = this->events[this->eventCount];
    this->eventCount--;
  }

  return 0;
}

auto Manager::onEvent(const Event* event) const -> int {
  int count = 0;

  for (int i = 0; i < this->eventCount; i++) {
    auto currentEvent = this->events[i];
    if (currentEvent == event->getType()) {
      int result = 0;
      auto subscribers = this->subscribers[i];
      for (auto& subscriber : subscribers) {
        if (subscriber == nullptr) continue;

        if (subscriber->isActive()) {
          result = subscriber->eventHandler(event);
          count++;
          if (result > 0) break;
        }
      }
    }
  }

  return count;
}

auto Manager::isValid([[maybe_unused]] string eventType) const -> bool {
  return false;
}

}  // namespace lb
