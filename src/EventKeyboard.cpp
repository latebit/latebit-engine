#include "EventKeyboard.h"

namespace lb {
EventKeyboard::EventKeyboard() { setType(KEYBOARD_EVENT); }

EventKeyboard::EventKeyboard(Keyboard::Key k, EventKeyboardAction a) {
  setType(KEYBOARD_EVENT);
  this->key = k;
  this->action = a;
}

void EventKeyboard::setKey(Keyboard::Key key) { this->key = key; }

auto EventKeyboard::getKey() const -> Keyboard::Key { return this->key; }

void EventKeyboard::setKeyboardAction(EventKeyboardAction action) {
  this->action = action;
}

auto EventKeyboard::getKeyboardAction() const -> EventKeyboardAction {
  return this->action;
}

}  // namespace lb
