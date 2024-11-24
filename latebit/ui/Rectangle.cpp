#include "Rectangle.h"

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Animation.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Object.h"

namespace lbui {
Rectangle::Rectangle(RectangleOptions options) : options(std::move(options)) {
  this->setBox({(float)options.width, (float)options.height});
  
  // UI elements do not participate in collisions and animations
  this->setSolidness(lb::Solidness::SPECTRAL);
  auto a = this->getAnimation();
  a.setSlowdownCount(STOP_ANIMATION_SLOWDOWN);
  this->setAnimation(a);
}

auto Rectangle::draw() -> int {
  auto box = this->getBox();
  auto strokes = (float)options.stroke.size();
  int result = 0;

  // This looks crazy, but drawing whole rectangles reduces the amount of
  // drawing calls compared to draw borders one by one (i.e., 4 times less
  // calls) which is more efficient.
  for (float i = 0; i < strokes; i++) {
    auto color = options.stroke[i];
    if (color == Color::UNDEFINED_COLOR) color = options.fill;

    result +=
      DM.fillRectangle(getPosition() + Vector{i, i},
                       box.getWidth() - 2 * i, box.getHeight() - 2 * i, color);
  }

  result += DM.fillRectangle(
    getPosition() + Vector{strokes, strokes},
    box.getWidth() - 2 * strokes, box.getHeight() - 2 * strokes, options.fill);

  return result;
};
}  // namespace lbui