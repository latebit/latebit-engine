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
  EventMouseAction mouse_action;
  Mouse::Button mouse_button;
  Vector mouse_position;

 public:
  EventMouse();
  EventMouse(EventMouseAction a, Mouse::Button b, Vector p);

  void setMouseAction(EventMouseAction a);

  [[nodiscard]] auto getMouseAction() const -> EventMouseAction;

  void setMouseButton(Mouse::Button b);

  [[nodiscard]] auto getMouseButton() const -> Mouse::Button;

  void setMousePosition(Vector p);

  [[nodiscard]] auto getMousePosition() const -> Vector;
};

}  // namespace df
