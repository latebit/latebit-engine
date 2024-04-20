#include "latebit/core/events/EventMouse.h"

#include "../../../test/lib/test.h"

using namespace lb;

auto main() -> int {
  test("constructor", []() {
    EventMouse event;

    assertEq("sets default mouse button", event.getMouseButton(),
             Mouse::UNDEFINED_MOUSE_BUTTON);

    assertEq("sets default mouse action", event.getMouseAction(),
             UNDEFINED_MOUSE_ACTION);
    assertEq("sets default mouse position", event.getMousePosition(), Vector());
  });

  test("constructor with parameters", []() {
    EventMouse event(CLICKED, Mouse::LEFT, Vector(1, 2));
    assertEq("sets correct mouse button", event.getMouseButton(), Mouse::LEFT);
    assertEq("sets correct mouse action", event.getMouseAction(), CLICKED);
    assertEq("sets correct mouse position", event.getMousePosition(),
             Vector(1, 2));
  });

  test("setters", []() {
    EventMouse event;
    event.setMouseButton(Mouse::RIGHT);
    assertEq("sets correct mouse button", event.getMouseButton(), Mouse::RIGHT);

    event.setMouseAction(MOVED);
    assertEq("sets correct mouse action", event.getMouseAction(), MOVED);

    event.setMousePosition(Vector(3, 4));
    assertEq("sets correct mouse position", event.getMousePosition(),
             Vector(3, 4));
  });

  return report();
}