#include "core/events/EventKeyboard.h"

#include "../lib/test.h"

void EventKeyboard_test() {
  test("constructor", []() {
    EventKeyboard event;
    assert_int("sets default key", event.getKey(), Keyboard::UNDEFINED_KEY);
    assert_int("sets default keyboard action", event.getKeyboardAction(),
               UNDEFINED_KEYBOARD_ACTION);
    assert_string("sets correct type", event.getType(), KEYBOARD_EVENT);
  });

  test("constructor with parameters", []() {
    EventKeyboard event(Keyboard::SPACE, KEY_PRESSED);
    assert_int("sets correct key", event.getKey(), Keyboard::SPACE);
    assert_int("sets correct keyboard action", event.getKeyboardAction(),
               KEY_PRESSED);
  });

  test("setters", []() {
    EventKeyboard event;

    event.setKey(Keyboard::SPACE);
    assert_int("sets correct key", event.getKey(), Keyboard::SPACE);

    event.setKeyboardAction(KEY_PRESSED);
    assert_int("sets correct keyboard action", event.getKeyboardAction(),
               KEY_PRESSED);
  });
}