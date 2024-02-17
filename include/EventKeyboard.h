#pragma once

#include "Event.h"

namespace lb {

const std::string KEYBOARD_EVENT = "lb::keyboard";

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
  // The key value
  Keyboard::Key key = Keyboard::UNDEFINED_KEY;
  // The key action
  EventKeyboardAction action = UNDEFINED_KEYBOARD_ACTION;

 public:
  EventKeyboard();
  // Create keyboard event with key value corresponding to the button pressed
  // and an action
  EventKeyboard(Keyboard::Key k, EventKeyboardAction a);

  // Update the key value for this event
  void setKey(Keyboard::Key key);
  // Return the key value for this event
  [[nodiscard]] auto getKey() const -> Keyboard::Key;

  // Update the action for this event
  void setKeyboardAction(EventKeyboardAction action);
  // Return the action for this event
  [[nodiscard]] auto getKeyboardAction() const -> EventKeyboardAction;
};

}  // namespace lb
