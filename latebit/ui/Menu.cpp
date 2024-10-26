#include "Menu.h"

#include "latebit/core/events/EventInput.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/ui/Control.h"

using namespace lb;

namespace lbui {
Menu::Menu() { subscribe(INPUT_EVENT); }

void Menu::focusItem(int index) {
  auto text = this->controls.at(index);
  auto label = text->getLabel();
  label.replace(0, 1, BULLET);
  text->setLabel(label);
}

void Menu::blurItem(int index) {
  auto text = this->controls.at(index);
  auto label = text->getLabel();
  label.replace(0, 1, " ");
  text->setLabel(label);
}

auto Menu::eventHandler(const Event *e) -> int {
  if (e->getType() == INPUT_EVENT) {
    auto event = static_cast<const EventInput *>(e);
    if (event->getAction() == InputAction::PRESSED) {
      if (event->getKey() == InputKey::DOWN) {
        blurItem(selected);
        selected = (selected + 1) % controls.size();
        focusItem(selected);
        return 1;
      } else if (event->getKey() == InputKey::UP) {
        blurItem(selected);
        selected = (selected - 1) % controls.size();
        focusItem(selected);
        return 1;
      } else if (event->getKey() == InputKey::START) {
        auto item = controls.at(selected);
        if (item) item->onSubmit();
        return 1;
      }
    }
  }
  return 0;
}

void Menu::addControl(Control *control) {
  auto box = this->getBox();
  auto itemPosition = this->getPosition() + Vector(0, box.getHeight() + GAP);
  control->setPosition(itemPosition);
  this->controls.push_back(control);
  control->setLabel("  " + control->getLabel());

  auto itemBox = control->getBox();
  auto width =
    box.getWidth() > itemBox.getWidth() ? box.getWidth() : itemBox.getWidth();
  auto height = box.getHeight() + itemBox.getHeight() + GAP;
  box.setWidth(width);
  box.setHeight(height);
  this->setBox(box);

  if (controls.size() == 1) {
    focusItem(0);
  }
}

void Menu::setPosition(Vector p) {
  // Watch out! This is order-dependent
  auto delta = p - this->getPosition();
  Object::setPosition(p);
  for (auto &item : controls) {
    item->setPosition(item->getPosition() + delta);
  }
}

void Menu::setDebug(bool debug) {
  Object::setDebug(debug);
  for (auto &item : controls) {
    item->setDebug(debug);
  }
}
}  // namespace lbui