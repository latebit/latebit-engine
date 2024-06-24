#include "EventInput.h"

namespace lb {

auto EventInput::getKey() const -> InputKey::InputKey { return this->key; }

auto EventInput::getAction() const -> InputAction::InputAction {
  return this->action;
}

}  // namespace lb
