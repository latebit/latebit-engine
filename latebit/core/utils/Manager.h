#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>

#include "latebit/core/events/Event.h"
#include "latebit/core/world/Object.h"

using namespace std;

namespace lb {
const int MAX_EVENTS = 100;

class Manager {
 private:
  const string type = "Manager";
  bool started = false;

  // Number of events for which there is a subscription
  int eventCount = 0;

  // The following are parallel lists: for a given i there is
  // a list subscriber[i] of objects subscribed to the event[i].

  // List of subscriptions. Every element matches an object in `subscribers`
  array<string, MAX_EVENTS> events = {};

  // List of subscribers. Every element matches an event in `events`
  array<vector<Object *>, MAX_EVENTS> subscribers = {};

 public:
  Manager(string type) : type(std::move(type)) {};
  virtual ~Manager() = default;

  // Returns the manager type
  [[nodiscard]] auto getType() const -> string;
  // Returns true when the manager has started up
  [[nodiscard]] auto isStarted() const -> bool;

  // Defers initialization of the manager, used to allow singleton behaviour
  virtual auto startUp() -> int;
  // Defers destruction of the manager, used to allow ingleton behaviour
  virtual void shutDown();

  // Validates whether a given event can be handled by this Manager
  [[nodiscard]] virtual auto isValid(string eventType) const -> bool;

  // Broadcasts the event to all the interested subscribers.
  // Returns count of events sent.
  auto onEvent(const Event *event) const -> int;

  // Register interest in an event type, to be notified when
  // events of that type occur
  auto subscribe(Object *o, string eventType) -> int;

  // Unregister interest in a given event type and stop being
  // notified
  auto unsubscribe(Object *o, string eventType) -> int;
};
};  // namespace lb
