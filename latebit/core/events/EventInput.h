#pragma once

#include "Event.h"
#include "latebit/core/input/InputManager.h"

using namespace std;

namespace lb {
const string INPUT_EVENT = "lb::input";

class EventInput : public Event {
 private:
  // The key that triggered the event
  const InputKey::InputKey key = InputKey::UNDEFINED_KEY;
  // The action resulting
  const InputAction::InputAction action = InputAction::UNDEFINED_ACTION;
  // The event type
  const string type = INPUT_EVENT;

 public:
  EventInput()
    : Event(INPUT_EVENT),
      key(InputKey::UNDEFINED_KEY),
      action(InputAction::UNDEFINED_ACTION) {};

  // Create input event with an InputKey representing the control being acted
  // on, and an action representing the action on the given control
  EventInput(InputKey::InputKey k, InputAction::InputAction a)
    : Event(INPUT_EVENT), key(k), action(a) {};

  // Return the key value for this event
  [[nodiscard]] auto getKey() const -> InputKey::InputKey;

  // Return the action for this event
  [[nodiscard]] auto getAction() const -> InputAction::InputAction;
};

}  // namespace lb
