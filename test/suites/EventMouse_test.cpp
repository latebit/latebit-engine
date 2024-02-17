#include "EventMouse.h"

#include "../lib/test.h"

using namespace lb;

void EventMouse_test() {
  test("constructor", []() {
    EventMouse event;

    assert_int("sets default mouse button", event.getMouseButton(),
               Mouse::UNDEFINED_MOUSE_BUTTON);

    assert_int("sets default mouse action", event.getMouseAction(),
               UNDEFINED_MOUSE_ACTION);
    assert_vector("sets default mouse position", event.getMousePosition(),
                  Vector());
  });

  test("constructor with parameters", []() {
    EventMouse event(CLICKED, Mouse::LEFT, Vector(1, 2));
    assert_int("sets correct mouse button", event.getMouseButton(),
               Mouse::LEFT);
    assert_int("sets correct mouse action", event.getMouseAction(), CLICKED);
    assert_vector("sets correct mouse position", event.getMousePosition(),
                  Vector(1, 2));
  });

  test("setters", []() {
    EventMouse event;
    event.setMouseButton(Mouse::RIGHT);
    assert_int("sets correct mouse button", event.getMouseButton(),
               Mouse::RIGHT);

    event.setMouseAction(MOVED);
    assert_int("sets correct mouse action", event.getMouseAction(), MOVED);

    event.setMousePosition(Vector(3, 4));
    assert_vector("sets correct mouse position", event.getMousePosition(),
                  Vector(3, 4));
  });
}