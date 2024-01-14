#include "EventKeyboard.h"

namespace df {
EventKeyboard::EventKeyboard() {
  setType(KEYBOARD_EVENT);
  this->key_val = Keyboard::UNDEFINED_KEY;
  this->keyboard_action = EventKeyboardAction::UNDEFINED_KEYBOARD_ACTION;
}

EventKeyboard::EventKeyboard(Keyboard::Key k, EventKeyboardAction a) {
  setType(KEYBOARD_EVENT);
  this->key_val = k;
  this->keyboard_action = a;
}

void EventKeyboard::setKey(Keyboard::Key new_key) { this->key_val = new_key; }

Keyboard::Key EventKeyboard::getKey() const { return this->key_val; }

void EventKeyboard::setKeyboardAction(EventKeyboardAction new_action) {
  this->keyboard_action = new_action;
}

EventKeyboardAction EventKeyboard::getKeyboardAction() const {
  return this->keyboard_action;
}

}  // namespace df
