#pragma once

#include "Event.h"

namespace df {

const std::string KEYBOARD_EVENT = "df::keyboard";

enum EventKeyboardAction {
  UNDEFINED_KEYBOARD_ACTION = -1,
  KEY_PRESSED,
  KEY_RELEASED,
};

namespace Keyboard {
enum Key {
  UNDEFINED_KEY = -1,
  SPACE,
  RETURN,
  ESCAPE,
  TAB,
  LEFTARROW,
  RIGHTARROW,
  UPARROW,
  DOWNARROW,
  PAUSE,
  MINUS,
  PLUS,
  TILDE,
  PERIOD,
  COMMA,
  SLASH,
  EQUAL,
  BACKSLASH,
  MULTIPLY,
  QUOTE,
  SEMICOLON,
  LEFTCONTROL,
  RIGHTCONTROL,
  LEFTSHIFT,
  RIGHTSHIFT,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  NUM1,
  NUM2,
  NUM3,
  NUM4,
  NUM5,
  NUM6,
  NUM7,
  NUM8,
  NUM9,
  NUM0,
  BACKSPACE,
};
}

class EventKeyboard : public Event {
 private:
  Keyboard::Key m_key_val;                ///< Key value.
  EventKeyboardAction m_keyboard_action;  ///< Key action.

 public:
  EventKeyboard();
  EventKeyboard(Keyboard::Key k, EventKeyboardAction a);

  void setKey(Keyboard::Key new_key);

  Keyboard::Key getKey() const;

  void setKeyboardAction(EventKeyboardAction new_action);

  EventKeyboardAction getKeyboardAction() const;
};

}  // namespace df
