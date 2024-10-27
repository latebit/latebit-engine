#include "Menu.h"

#include "latebit/core/events/EventInput.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/Object.h"
#include "latebit/ui/Control.h"

using namespace lb;

namespace lbui {
// Space between elements in the menu
const int GAP = 4;

const Icon Menu::CARET = Icon(
  {Color::WHITE, Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR,
   Color::WHITE, Color::WHITE, Color::WHITE,           Color::UNDEFINED_COLOR,
   Color::WHITE, Color::WHITE, Color::WHITE,           Color::WHITE,
   Color::WHITE, Color::WHITE, Color::WHITE,           Color::UNDEFINED_COLOR,
   Color::WHITE, Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR},
  4, 5, 1);

Menu::Menu() { subscribe(INPUT_EVENT); }

auto Menu::draw() -> int { return CARET.draw(caretPosition); }

void Menu::blurItem(int index) {
  auto control = this->controls.at(index);
  if (control) control->onBlur();
}

void Menu::focusItem(int index) {
  auto control = this->controls.at(index);
  if (control) {
    auto position = control->getPosition();
    this->caretPosition = position - Vector{(float)CARET.getWidth() + GAP, -1};
    control->onFocus();
  }
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
      } else if (event->getKey() == InputKey::LEFT) {
        auto item = controls.at(selected);
        if (item) item->onChange(-1);
        return 1;
      } else if (event->getKey() == InputKey::RIGHT) {
        auto item = controls.at(selected);
        if (item) item->onChange(1);
        return 1;
      }
    }
  }
  return 0;
}

void Menu::addControl(Control *control) {
  auto box = this->getBox();
  auto itemPosition =
    this->getPosition() + Vector(CARET.getWidth() + GAP, box.getHeight() + GAP);
  control->setPosition(itemPosition);
  this->controls.push_back(control);

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
  this->caretPosition = this->caretPosition + delta;
}

void Menu::setDebug(bool debug) {
  Object::setDebug(debug);
  for (auto &item : controls) {
    item->setDebug(debug);
  }
}
}  // namespace lbui