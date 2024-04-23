#include "Manager.h"

#include "latebit/core/events/Event.h"
#include "latebit/core/objects/ObjectListIterator.h"

namespace lb {

void Manager::setType(std::string type) { this->type = type; }

Manager::Manager() = default;
Manager::~Manager() = default;

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
      this->subscribers[i].insert(o);
      return 0;
    }
  }

  // Subscribe to a new event
  this->events[this->eventCount] = eventType;
  this->subscribers[this->eventCount].clear();
  this->subscribers[this->eventCount].insert(o);
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
      this->subscribers[i].remove(o);
      foundIndex = i;
      break;
    }
  }

  if (this->subscribers[foundIndex].isEmpty()) {
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
      // Generate a copy to avoid race conditions on the subscribers list
      // For example when as a result of handling an event, a new subscriber
      // is added to the list
      auto subscribers = this->subscribers[i];
      auto iterator = ObjectListIterator(&subscribers);

      for (iterator.first(); !iterator.isDone(); iterator.next()) {
        auto currentObject = iterator.currentObject();
        if (currentObject == nullptr) continue;

        if (currentObject->isActive()) {
          currentObject->eventHandler(event);
          count++;
        }
      }
    }
  }

  return count;
}

auto Manager::isValid(string eventType) const -> bool { return false; }

}  // namespace lb
