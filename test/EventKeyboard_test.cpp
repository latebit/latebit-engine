#include "../include/EventKeyboard.h"

#include "test.h"
int EventKeyboard_test() {
  int result = 0;

  // Test default constructor
  df::EventKeyboard df_event_keyboard;
  result += assert_int("sets default key", df_event_keyboard.getKey(),
                       df::Keyboard::UNDEFINED_KEY);
  result += assert_int("sets default keyboard action",
                       df_event_keyboard.getKeyboardAction(),
                       df::UNDEFINED_KEYBOARD_ACTION);
  result += assert_string("sets correct type", df_event_keyboard.getType(),
                          df::KEYBOARD_EVENT);

  // Test constructor with parameters
  df::EventKeyboard df_event_keyboard2(df::Keyboard::SPACE, df::KEY_PRESSED);
  result += assert_int("sets correct key", df_event_keyboard2.getKey(),
                       df::Keyboard::SPACE);
  result += assert_int("sets correct keyboard action",
                       df_event_keyboard2.getKeyboardAction(), df::KEY_PRESSED);

  // Test setKey()
  df_event_keyboard.setKey(df::Keyboard::SPACE);
  result += assert_int("sets correct key", df_event_keyboard.getKey(),
                       df::Keyboard::SPACE);

  // Test setKeyboardAction()
  df_event_keyboard.setKeyboardAction(df::KEY_PRESSED);
  result += assert_int("sets correct keyboard action",
                       df_event_keyboard.getKeyboardAction(), df::KEY_PRESSED);

  return 0;
}