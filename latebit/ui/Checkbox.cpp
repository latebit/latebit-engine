#include "latebit/ui/Checkbox.h"
#include "Checkbox.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/ui/Control.h"

namespace lbui {
// Space between the checkbox and the label 
const int GAP = 4;
// Size of the checkbox 
const int CHECK_WIDTH = 5;
const int CHECK_HEIGHT = 5;

// Frames representing the check, first checked, second unchecked
const array<Keyframe, 2> Checkbox::CHECKS = {
  Keyframe{
    Color::WHITE, Color::WHITE,           Color::WHITE,           Color::WHITE,           Color::WHITE, 
    Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::WHITE, 
    Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::WHITE,
    Color::WHITE, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::WHITE, 
    Color::WHITE, Color::WHITE,           Color::WHITE,           Color::WHITE,           Color::WHITE 
  },
  Keyframe{
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,Color::WHITE,
  }
};

Checkbox::Checkbox(Scene* scene, const std::string label, bool initialValue,
               std::function<void()> onSubmit) {
  this->text = scene->createObject<Text>(label, label);
  auto textBox = this->text->getBox();
  textBox.setWidth(textBox.getWidth() + GAP + CHECK_WIDTH);
  this->text->setPosition(this->getPosition() + Vector{CHECK_WIDTH + GAP, 0});
  this->setBox(textBox);
  this->callback = onSubmit;
  this->value = initialValue;
}

auto Checkbox::draw() -> int {
  return DM.drawKeyframe(getPosition() + Vector{0, 1}, &Checkbox::CHECKS[this->value], CHECK_WIDTH, CHECK_HEIGHT, 1);
}

auto Checkbox::getLabel() -> std::string { return this->text->getContent(); }

void Checkbox::setLabel(const std::string l) {
  this->text->setContent(l);
  this->setBox(text->getBox());
}

void Checkbox::setPosition(Vector p) {
  Control::setPosition(p);
  this->text->setPosition(p + Vector{CHECK_WIDTH + GAP, 0});
}

void Checkbox::onSubmit() { 
  this->value = !this->value;
  this->callback();
}

auto Checkbox::getValue() -> bool { return this->value; }

}  // namespace lbui