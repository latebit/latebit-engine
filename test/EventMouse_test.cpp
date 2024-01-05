#include "../includes/EventMouse.h"

#include "test.h"

int EventMouse_test() {
  int result = 0;

  // test default constructor
  df::EventMouse df_event_mouse;
  result +=
      assert_int("sets default mouse button", df_event_mouse.getMouseButton(),
                 df::Mouse::UNDEFINED_MOUSE_BUTTON);
  result +=
      assert_int("sets default mouse action", df_event_mouse.getMouseAction(),
                 df::UNDEFINED_MOUSE_ACTION);
  result += assert_vector("sets default mouse position",
                          df_event_mouse.getMousePosition(), df::Vector());

  // test constructor with parameters
  df::EventMouse df_event_mouse2(df::CLICKED, df::Mouse::LEFT,
                                 df::Vector(1, 2));
  result += assert_int("sets correct mouse button",
                       df_event_mouse2.getMouseButton(), df::Mouse::LEFT);
  result += assert_int("sets correct mouse action",
                       df_event_mouse2.getMouseAction(), df::CLICKED);
  result += assert_vector("sets correct mouse position",
                          df_event_mouse2.getMousePosition(), df::Vector(1, 2));

  // test setMouseButton()
  df_event_mouse.setMouseButton(df::Mouse::RIGHT);
  result += assert_int("sets correct mouse button",
                       df_event_mouse.getMouseButton(), df::Mouse::RIGHT);
  // test setMouseAction()
  df_event_mouse.setMouseAction(df::MOVED);
  result += assert_int("sets correct mouse action",
                       df_event_mouse.getMouseAction(), df::MOVED);
  // test setMousePosition()
  df_event_mouse.setMousePosition(df::Vector(3, 4));
  result += assert_vector("sets correct mouse position",
                          df_event_mouse.getMousePosition(), df::Vector(3, 4));

  return result;
}