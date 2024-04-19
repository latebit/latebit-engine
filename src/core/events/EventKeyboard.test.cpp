#include "core/events/EventKeyboard.h"

#include "../../../test/lib/test.h"

auto main() -> int {
  test("constructor", []() {
    EventKeyboard event;
    assertEq("sets default key", event.getKey(), Keyboard::UNDEFINED_KEY);
    assertEq("sets default keyboard action", event.getKeyboardAction(),
             UNDEFINED_KEYBOARD_ACTION);
    assertEq("sets correct type", event.getType(), KEYBOARD_EVENT);
  });

  test("constructor with parameters", []() {
    EventKeyboard event(Keyboard::SPACE, KEY_PRESSED);
    assertEq("sets correct key", event.getKey(), Keyboard::SPACE);
    assertEq("sets correct keyboard action", event.getKeyboardAction(),
             KEY_PRESSED);
  });

  test("setters", []() {
    EventKeyboard event;

    event.setKey(Keyboard::SPACE);
    assertEq("sets correct key", event.getKey(), Keyboard::SPACE);

    event.setKeyboardAction(KEY_PRESSED);
    assertEq("sets correct keyboard action", event.getKeyboardAction(),
             KEY_PRESSED);
  });

  return report();
}