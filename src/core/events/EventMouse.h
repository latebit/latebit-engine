#pragma once

#include "Event.h"
#include "Vector.h"

using namespace std;

namespace lb {

// MOUSE_EVENT would conflict with a macro in Windows
const string MSE_EVENT = "lb::mouse";

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
  EventMouseAction action = UNDEFINED_MOUSE_ACTION;
  Mouse::Button button = Mouse::UNDEFINED_MOUSE_BUTTON;
  Vector position = Vector();

 public:
  EventMouse();
  // Creates a mouse event with the given action and button occurring at a given
  // position
  EventMouse(EventMouseAction a, Mouse::Button b, Vector p);

  // Sets the action of the mouse event
  void setMouseAction(EventMouseAction a);
  // Returns the action of the mouse event
  [[nodiscard]] auto getMouseAction() const -> EventMouseAction;

  // Sets the button of the mouse event
  void setMouseButton(Mouse::Button b);
  // Returns the button of the mouse event
  [[nodiscard]] auto getMouseButton() const -> Mouse::Button;

  // Sets the position of the mouse event
  void setMousePosition(Vector p);
  // Returns the position of the mouse event
  [[nodiscard]] auto getMousePosition() const -> Vector;
};

}  // namespace lb
