#include "EventKeyboard.h"

namespace df {
EventKeyboard::EventKeyboard() {
  setType(KEYBOARD_EVENT);
  m_key_val = Keyboard::UNDEFINED_KEY;
  m_keyboard_action = EventKeyboardAction::UNDEFINED_KEYBOARD_ACTION;
}

EventKeyboard::EventKeyboard(Keyboard::Key k, EventKeyboardAction a) {
  setType(KEYBOARD_EVENT);
  m_key_val = k;
  m_keyboard_action = a;
}

void EventKeyboard::setKey(Keyboard::Key new_key) { m_key_val = new_key; }

Keyboard::Key EventKeyboard::getKey() const { return m_key_val; }

void EventKeyboard::setKeyboardAction(EventKeyboardAction new_action) {
  m_keyboard_action = new_action;
}

EventKeyboardAction EventKeyboard::getKeyboardAction() const {
  return m_keyboard_action;
}

}  // namespace df
