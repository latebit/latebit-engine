#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
struct RectangleOptions {
  uint8_t height = 0;
  uint8_t width = 0;
  Color::Color fill = Color::UNDEFINED_COLOR;
  Keyframe stroke = {};
};

class Rectangle : public Object {
  RectangleOptions options;

 public:
  Rectangle(RectangleOptions options = RectangleOptions());
  auto draw() -> int override;
};
}  // namespace lbui