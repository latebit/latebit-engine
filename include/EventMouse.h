#pragma once

#include "Event.h"
#include "Vector.h"

namespace df {

// MOUSE_EVENT would conflict with a macro in Windows
const std::string MSE_EVENT = "df-mouse";

enum EventMouseAction {
  UNDEFINED_MOUSE_ACTION = -1,
  CLICKED,
  MOVED,
};

namespace Mouse {
enum Button {
  UNDEFINED_MOUSE_BUTTON = -1,
  LEFT,
  RIGHT,
  MIDDLE,
};
}

class EventMouse : public Event {
 private:
  EventMouseAction m_mouse_action;
  Mouse::Button m_mouse_button;
  Vector m_mouse_position;

 public:
  EventMouse();
  EventMouse(EventMouseAction a, Mouse::Button b, Vector p);

  void setMouseAction(EventMouseAction a);

  EventMouseAction getMouseAction() const;

  void setMouseButton(Mouse::Button b);

  Mouse::Button getMouseButton() const;

  void setMousePosition(Vector p);

  Vector getMousePosition() const;
};

}  // namespace df
