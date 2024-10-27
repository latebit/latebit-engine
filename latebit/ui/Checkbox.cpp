#include "latebit/ui/Checkbox.h"
#include "Checkbox.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Icon.h"

namespace lbui {
// Space between the checkbox and the label 
const int GAP = 4;

const int CHECK_SIZE = 5;

// Frames representing the check, first checked, second unchecked
// clang-format off
const array<Icon, 2> Checkbox::CHECKS = {
  Icon({
    Color::WHITE, Color::WHITE,           Color::WHITE,           Color::WHITE,           Color::WHITE, 
    Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::WHITE, 
    Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::WHITE,
    Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::WHITE, 
    Color::WHITE, Color::WHITE,           Color::WHITE,           Color::WHITE,           Color::WHITE 
  }, CHECK_SIZE, CHECK_SIZE),
  Icon({
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE,
  }, CHECK_SIZE, CHECK_SIZE)
};
// clang-format on

Checkbox::Checkbox(Scene* scene, const std::string label, bool initialValue,
               std::function<void(bool)> onSubmit) {
  this->text = scene->createObject<Text>(label, label);
  auto textBox = this->text->getBox();
  textBox.setWidth(textBox.getWidth() + GAP + CHECK_SIZE);
  this->text->setPosition(this->getPosition() + Vector{CHECK_SIZE + GAP, 0});
  this->setBox(textBox);
  this->callback = onSubmit;
  this->value = initialValue;
}

auto Checkbox::draw() -> int {
  return Checkbox::CHECKS[this->value].draw(getPosition() + Vector{0, 1});
}

auto Checkbox::getLabel() -> std::string { return this->text->getContent(); }

void Checkbox::setLabel(const std::string l) {
  this->text->setContent(l);
  this->setBox(text->getBox());
}

void Checkbox::setPosition(Vector p) {
  Control::setPosition(p);
  this->text->setPosition(p + Vector{CHECK_SIZE + GAP, 0});
}

void Checkbox::onSubmit() { 
  this->value = !this->value;
  this->callback(this->value);
}

auto Checkbox::getValue() -> bool { return this->value; }

}  // namespace lbui