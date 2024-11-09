#include "Text.h"

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
Text::Text(const string type, const string text, const TextOptions options)
  : Object(type) {
  this->options = options;
  this->setContent(text);
  this->setBodyType(lb::BodyType::KINEMATIC);
}

auto Text::draw() -> int {
  int result = 0;
  auto position = getPosition();

  if (options.shadow != Color::UNDEFINED_COLOR) {
    result += DM.drawString(position + SHADOW_POSITION, content,
                            TextAlignment::LEFT, options.shadow, options.size);
  }

  if (this->getDebug()) {
    result += Object::drawBoundingBox();
  }

  return DM.drawString(position, content, TextAlignment::LEFT, options.color,
                       options.size) +
         result;
}

void Text::setContent(const string text) {
  this->content = text;

  auto box = DM.measureString(this->content, options.size);
  this->setBox(box);
}

void Text::setPosition(Vector position) {
  auto box = this->getBox();
  if (options.alignment == TextAlignment::CENTER) {
    position.setX(position.getX() - box.getWidth() / 2);
  } else if (options.alignment == TextAlignment::RIGHT) {
    position.setX(position.getX() - box.getWidth());
  }
  Object::setPosition(position);
}

auto Text::getContent() -> string { return this->content; }
};  // namespace lbui