#include "Manager.h"

#include "Event.h"
#include "ObjectListIterator.h"

namespace df {

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
  // if (!isValid(eventType)) {
  //   return -1;
  // }

  for (int i = 0; i < eventCount; i++) {
    if (events[i] == eventType) {
      subscribers[i].insert(o);
      return 0;
    }
  }

  if (eventCount >= MAX_EVENTS) return -1;

  events[eventCount] = eventType;
  subscribers[eventCount].clear();
  subscribers[eventCount].insert(o);
  eventCount++;

  return 0;
}

auto Manager::unsubscribe(Object* o, string eventType) -> int {
  int foundIndex = 0;
  for (int i = 0; i < eventCount; i++) {
    if (events[i] == eventType) {
      subscribers[i].remove(o);
      foundIndex = i;
      break;
    }
  }

  if (subscribers[foundIndex].isEmpty()) {
    subscribers[foundIndex] = subscribers[eventCount];
    events[foundIndex] = events[eventCount];
    eventCount--;
  }

  return 0;
}

auto Manager::onEvent(const Event* event) const -> int {
  int count = 0;

  for (int i = 0; i < eventCount; i++) {
    auto currentEvent = events[i];
    if (currentEvent == event->getType()) {
      auto iterator = ObjectListIterator(&subscribers[i]);

      for (iterator.first(); !iterator.isDone(); iterator.next()) {
        if (iterator.currentObject() == nullptr) continue;

        iterator.currentObject()->eventHandler(event);
        count++;
      }
    }
  }

  return count;
}

}  // namespace df
