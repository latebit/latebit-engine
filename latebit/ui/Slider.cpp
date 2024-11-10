#include "Slider.h"

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/ui/Control.h"
#include "latebit/utils/Math.h"

namespace lbui {

// Space between the text and the bars
const int GAP = 4;
// Space between the bars
const int BAR_GAP = 1;
// Number of bars to display
const int NUMBER_OF_BARS = 5;
// Width of every single bar
const int BAR_WIDTH = 3;
// Maximum value allowed
const int MAX = 5;
// Minimum value allowed
const int MIN = 0;

// clang-format off
const array<Icon, 2> Slider::BARS = {
  Icon({
    Color::WHITE, Color::WHITE,           Color::WHITE, 
    Color::WHITE, Color::UNDEFINED_COLOR, Color::WHITE, 
    Color::WHITE, Color::UNDEFINED_COLOR, Color::WHITE,
    Color::WHITE, Color::UNDEFINED_COLOR, Color::WHITE, 
    Color::WHITE, Color::WHITE,           Color::WHITE,
  }, BAR_WIDTH, 5),
  Icon({
    Color::WHITE, Color::WHITE, Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE,
    Color::WHITE, Color::WHITE, Color::WHITE, 
    Color::WHITE, Color::WHITE, Color::WHITE,
  }, BAR_WIDTH, 5)
};
// clang-format on

Slider::Slider(Scene* scene, const string label, int initialValue,
               function<void(int, int)> onChanged) {
  this->text = scene->createObject<Text>(label, label);
  auto textBox = this->text->getBox();
  textBox.setWidth(textBox.getWidth() + GAP +
                   (BAR_GAP + BAR_WIDTH) * NUMBER_OF_BARS);
  this->text->setPosition(this->getPosition());
  this->setBox(textBox);
  this->callback = onChanged;
  this->value = initialValue;
}

auto Slider::getLabel() -> string { return this->text->getContent(); }

void Slider::setLabel(const string l) { this->text->setContent(l); }

void Slider::setPosition(Vector p) {
  Control::setPosition(p);
  this->text->setPosition(p);
}

void Slider::onChange(int v) {
  auto oldValue = this->value;
  this->value = clamp(this->value + v, MIN, MAX);
  this->callback(oldValue, this->value);
}

auto Slider::draw() -> int {
  auto position = this->text->getPosition() +
                  Vector{this->text->getBox().getWidth() + GAP, 0};
  int result = 0;
  for (int i = 0; i < NUMBER_OF_BARS; i++) {
    result += BARS[i < value].draw(position +
                                   Vector{(float)i * (BAR_GAP + BAR_WIDTH), 1});
  }
  return result;
}

auto Slider::getValue() -> int { return value; }

}  // namespace lbui