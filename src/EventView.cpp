#include "EventView.h"

namespace df {

EventView::EventView() { setType(VIEW_EVENT); }

EventView::EventView(string objectType, int value, bool delta) {
  setType(VIEW_EVENT);
  setObjectType(objectType);
  setValue(value);
  setDelta(delta);
}

void EventView::setObjectType(string type) { this->objectType = type; }
auto EventView::getObjectType() const -> string { return objectType; }

void EventView::setValue(int value) { this->value = value; }
auto EventView::getValue() const -> int { return value; }

void EventView::setDelta(bool delta) { this->delta = delta; }
auto EventView::getDelta() const -> bool { return delta; }

}  // namespace df