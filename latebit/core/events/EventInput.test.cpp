#include "latebit/core/events/EventInput.h"

#include "test/lib/test.h"

auto main() -> int {
  test("constructor", []() {
    EventInput event;
    assertEq("sets default key", event.getKey(), InputKey::UNDEFINED_KEY);
    assertEq("sets default keyboard action", event.getAction(),
             InputAction::UNDEFINED_ACTION);
    assertEq("sets correct type", event.getType(), INPUT_EVENT);
  });

  test("constructor with parameters", []() {
    EventInput event(InputKey::A, InputAction::PRESSED, true);
    assertEq("sets correct key", event.getKey(), InputKey::A);
    assertEq("sets correct keyboard action", event.getAction(),
             InputAction::PRESSED);
    assertEq("sets correct repeat", event.getRepeat(), true);
  });

  return report();
}