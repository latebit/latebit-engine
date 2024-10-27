#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
struct RectangleOptions {
  // Height of the rectangle
  uint8_t height = 0;
  // Width of the rectangle
  uint8_t width = 0;
  // Color of the rectangle
  Color::Color fill = Color::UNDEFINED_COLOR;
  // The pattern for the border of the rectangle. Every item represents a
  // 1-cell-wide border. So to have a border that is RED and twice as thick
  // BLUE, you specify, {RED, BLUE, BLUE}.
  Keyframe stroke = {};
};

class Rectangle : public Object {
  RectangleOptions options;

 public:
  Rectangle(RectangleOptions options = RectangleOptions());
  auto draw() -> int override;
};
}  // namespace lbui