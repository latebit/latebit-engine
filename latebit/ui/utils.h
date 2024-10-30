#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
namespace ScreenPosition {
enum ScreenPosition {
  TOP_LEFT,
  TOP_CENTER,
  TOP_RIGHT,
  MIDDLE_LEFT,
  MIDDLE_CENTER,
  MIDDLE_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_CENTER,
  BOTTOM_RIGHT
};
}

// Place an element in a certain position on the screen accounting for its
// bounding box
void setPositionOnScreen(Object* obj, ScreenPosition::ScreenPosition position);
// Place an element in a certain position on the screen accounting for its
// bounding box and an offset
void setPositionOnScreen(Object* obj, ScreenPosition::ScreenPosition position,
                         Vector offset);
}  // namespace lbui