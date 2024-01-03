#include "EventMouse.h"

namespace df {
EventMouse::EventMouse() {
  setType(MSE_EVENT);
  m_mouse_action = EventMouseAction::UNDEFINED_MOUSE_ACTION;
  m_mouse_button = Mouse::UNDEFINED_MOUSE_BUTTON;
  m_mouse_position = Vector();
}

EventMouse::EventMouse(EventMouseAction a, Mouse::Button b, Vector p) {
  m_mouse_action = a;
  m_mouse_button = b;
  m_mouse_position = p;
}

void EventMouse::setMouseAction(EventMouseAction new_mouse_action) {
  m_mouse_action = new_mouse_action;
}

EventMouseAction EventMouse::getMouseAction() const { return m_mouse_action; }

void EventMouse::setMouseButton(Mouse::Button new_mouse_button) {
  m_mouse_button = new_mouse_button;
}

Mouse::Button EventMouse::getMouseButton() const { return m_mouse_button; }

void EventMouse::setMousePosition(Vector new_mouse_xy) {
  m_mouse_position = new_mouse_xy;
}

Vector EventMouse::getMousePosition() const { return m_mouse_position; }

}  // namespace df
