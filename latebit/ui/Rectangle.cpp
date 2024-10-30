#include "Rectangle.h"

#include <cstdint>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"

namespace lbui {
Rectangle::Rectangle(RectangleOptions options) : options(std::move(options)) {
  this->setBox({(float)options.width, (float)options.height});
}

auto Rectangle::draw() -> int {
  auto box = this->getBox();
  auto strokes = options.stroke.size();
  int result = 0;

  // This looks crazy, but drawing whole rectangles reduces the amount of
  // drawing calls compared to draw borders one by one (i.e., 4 times less
  // calls) which is more efficient.
  for (uint8_t i = 0; i < strokes; i++) {
    auto color = options.stroke[i];
    if (color == Color::UNDEFINED_COLOR) color = options.fill;

    result +=
      DM.fillRectangle(getPosition() + Vector{(float)i, (float)i},
                       box.getWidth() - 2 * i, box.getHeight() - 2 * i, color);
  }

  result += DM.fillRectangle(
    getPosition() + Vector{(float)strokes, (float)strokes},
    box.getWidth() - 2 * strokes, box.getHeight() - 2 * strokes, options.fill);

  return result;
};
}  // namespace lbui