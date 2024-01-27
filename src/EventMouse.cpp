#include "EventMouse.h"

namespace df {
EventMouse::EventMouse() { setType(MSE_EVENT); }

EventMouse::EventMouse(EventMouseAction a, Mouse::Button b, Vector p) {
  setType(MSE_EVENT);
  this->action = a;
  this->button = b;
  this->position = p;
}

void EventMouse::setMouseAction(EventMouseAction action) {
  this->action = action;
}

auto EventMouse::getMouseAction() const -> EventMouseAction {
  return this->action;
}

void EventMouse::setMouseButton(Mouse::Button button) { this->button = button; }

auto EventMouse::getMouseButton() const -> Mouse::Button {
  return this->button;
}

void EventMouse::setMousePosition(Vector position) {
  this->position = position;
}

auto EventMouse::getMousePosition() const -> Vector { return this->position; }

}  // namespace df
