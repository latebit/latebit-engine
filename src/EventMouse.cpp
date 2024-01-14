#include "EventMouse.h"

namespace df {
EventMouse::EventMouse() {
  setType(MSE_EVENT);
  this->mouse_action = EventMouseAction::UNDEFINED_MOUSE_ACTION;
  this->mouse_button = Mouse::UNDEFINED_MOUSE_BUTTON;
  this->mouse_position = Vector();
}

EventMouse::EventMouse(EventMouseAction a, Mouse::Button b, Vector p) {
  this->mouse_action = a;
  this->mouse_button = b;
  this->mouse_position = p;
}

void EventMouse::setMouseAction(EventMouseAction new_mouse_action) {
  this->mouse_action = new_mouse_action;
}

EventMouseAction EventMouse::getMouseAction() const { return this->mouse_action; }

void EventMouse::setMouseButton(Mouse::Button new_mouse_button) {
  this->mouse_button = new_mouse_button;
}

Mouse::Button EventMouse::getMouseButton() const { return this->mouse_button; }

void EventMouse::setMousePosition(Vector new_mouse_xy) {
  this->mouse_position = new_mouse_xy;
}

Vector EventMouse::getMousePosition() const { return this->mouse_position; }

}  // namespace df
