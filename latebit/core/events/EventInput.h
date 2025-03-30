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
  // If true, the event is a repeat of a previous event. Used to detect long
  // pressed, for example
  const bool repeat = false;

 public:
  EventInput() : Event(INPUT_EVENT){};

  // Create input event with an InputKey representing the button being pressed,
  // whether it's a press or a release, and if it's repeated over time
  EventInput(InputKey::InputKey k, InputAction::InputAction a,
             bool repeat = false)
    : Event(INPUT_EVENT), key(k), action(a), repeat(repeat){};

  // Return the key value for this event
  [[nodiscard]] auto getKey() const -> InputKey::InputKey { return key; }

  // Return the action for this event
  [[nodiscard]] auto getAction() const -> InputAction::InputAction {
    return action;
  }

  // Return true if the event is repeated
  [[nodiscard]] auto getRepeat() const -> bool { return repeat; }
};

}  // namespace lb
