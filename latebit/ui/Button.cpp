#include "Button.h"

namespace lbui {

Button::Button(Scene* scene, const std::string label,
               std::function<void()> onSubmit) {
  this->text = scene->createObject<Text>(label, label);
  this->setBox(this->text->getBox());
  this->callback = onSubmit;
}

auto Button::getLabel() -> std::string { return this->text->getContent(); }

void Button::setLabel(const std::string l) {
  this->text->setContent(l);
  this->setBox(text->getBox());
}

void Button::setPosition(Vector p) {
  Control::setPosition(p);
  this->text->setPosition(p);
}

void Button::onSubmit() { this->callback(); }

}  // namespace lbui