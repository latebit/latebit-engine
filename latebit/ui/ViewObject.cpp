#include "ViewObject.h"

#include "latebit/core/events/EventView.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/objects/SceneGraph.h"
#include "latebit/core/objects/WorldManager.h"
#include "latebit/utils/Math.h"

namespace lb {

ViewObject::ViewObject() {
  setType("ViewObject");
  setSolidness(Solidness::SPECTRAL);
  setAltitude(MAX_ALTITUDE);

  subscribe(VIEW_EVENT);
}

auto ViewObject::getDisplayString() -> string {
  return label + ": " + to_string(value);
}

auto ViewObject::draw() -> int {
  return DM.drawString(getPosition(), getDisplayString(), TextAlignment::LEFT,
                       color);
}

auto ViewObject::eventHandler(const Event* p_e) -> int {
  if (p_e->getType() == VIEW_EVENT) {
    auto viewEvent = dynamic_cast<const EventView*>(p_e);
    if (viewEvent->getObjectType() == getType()) {
      if (viewEvent->getDelta()) {
        setValue(getValue() + viewEvent->getValue());
      } else {
        setValue(viewEvent->getValue());
      }
      return 1;
    }
  }
  return 0;
}

auto ViewObject::refresh() -> void {
  float textWidth = getBox().getWidth();
  float textHeight = getBox().getHeight();
  float viewWidth = WM.getView().getWidth();
  float viewHeight = WM.getView().getHeight();

  float x = 0, y = 0;
  switch (this->location) {
    case TOP_LEFT:
      break;
    case TOP_CENTER:
      x = (viewWidth / 2) - (textWidth / 2);
      break;
    case TOP_RIGHT:
      x = viewWidth - textWidth;
      break;
    case CENTER_LEFT:
      y = (viewHeight / 2) - (textHeight / 2);
      break;
    case CENTER_CENTER:
      x = (viewWidth / 2) - (textWidth / 2);
      y = (viewHeight / 2) - (textHeight / 2);
      break;
    case CENTER_RIGHT:
      x = viewWidth - textWidth;
      y = (viewHeight / 2) - (textHeight / 2);
      break;
    case BOTTOM_LEFT:
      x = 0;
      y = viewHeight - textHeight;
      break;
    case BOTTOM_CENTER:
      x = (viewWidth / 2) - (textWidth / 2);
      y = viewHeight - textHeight;
      break;
    case BOTTOM_RIGHT:
      x = viewWidth - textWidth;
      y = viewHeight - textHeight;
      break;
  }

  x = clamp(x, 0.0f, viewWidth - textWidth);
  y = clamp(y, 0.0f, viewHeight - textHeight);

  auto position = WorldManager::viewToWorld(Vector(x, y));
  setPosition(position);
}

auto ViewObject::setLocation(ViewObjectLocation l) -> void {
  this->location = l;
  this->refresh();
}
auto ViewObject::getLocation() const -> ViewObjectLocation { return location; }

auto ViewObject::setValue(int value) -> void {
  this->value = value;
  this->setBox(DM.measureString(getDisplayString()));
  this->refresh();
}
auto ViewObject::getValue() const -> int { return value; }

auto ViewObject::setColor(Color::Color color) -> void { this->color = color; }
auto ViewObject::getColor() const -> Color::Color { return color; }

auto ViewObject::setLabel(string label) -> void {
  this->label = label;
  this->setBox(DM.measureString(getDisplayString()));
  this->refresh();
}
auto ViewObject::getLabel() const -> string { return label; }

}  // namespace lb