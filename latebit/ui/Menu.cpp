#include "Menu.h"

#include "latebit/core/events/EventInput.h"

using namespace lb;

namespace lbui {
Menu::Menu(Scene *scene) : scene(scene) { subscribe(INPUT_EVENT); }

void Menu::focusItem(int index) {
  auto text = this->items.at(index);
  auto label = text->getContent();
  label.replace(0, 1, BULLET);
  text->setContent(label);
}

void Menu::blurItem(int index) {
  auto text = this->items.at(index);
  auto label = text->getContent();
  label.replace(0, 1, " ");
  text->setContent(label);
}

auto Menu::eventHandler(const Event *e) -> int {
  if (e->getType() == INPUT_EVENT) {
    auto event = static_cast<const EventInput *>(e);
    if (event->getAction() == InputAction::PRESSED) {
      if (event->getKey() == InputKey::DOWN) {
        blurItem(selected);
        selected = (selected + 1) % items.size();
        focusItem(selected);
        return 1;
      } else if (event->getKey() == InputKey::UP) {
        blurItem(selected);
        selected = (selected - 1) % items.size();
        focusItem(selected);
        return 1;
      } else if (event->getKey() == InputKey::START) {
        callbacks.at(selected)();
        return 1;
      }
    }
  }
  return 0;
}

void Menu::addItem(string label, function<void()> onSelected) {
  auto item = scene->createObject<Text>(this->getType() + "-" + label, "  " + label);
  auto box = this->getBox();
  auto itemPosition = this->getPosition() + Vector(0, box.getHeight() + GAP);
  item->setPosition(itemPosition);
  this->callbacks.push_back(onSelected);
  this->items.push_back(item);

  auto itemBox = item->getBox();
  auto width =
    box.getWidth() > itemBox.getWidth() ? box.getWidth() : itemBox.getWidth();
  auto height = box.getHeight() + itemBox.getHeight() + GAP;
  box.setWidth(width);
  box.setHeight(height);
  this->setBox(box);

  if (items.size() == 1) {
    focusItem(0);
  }
}

void Menu::setPosition(Vector p) {
  // Watch out! This is order-dependent
  auto delta = p - this->getPosition();
  Object::setPosition(p);
  for (auto &item : items) {
    item->setPosition(item->getPosition() + delta);
  }
}

void Menu::setDebug(bool debug) {
  Object::setDebug(debug);
  for (auto &item : items) {
    item->setDebug(debug);
  }
}
}  // namespace lbui