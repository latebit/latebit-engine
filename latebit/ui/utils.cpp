#include "utils.h"

#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
void setPositionOnScreen(Object* obj, ScreenPosition::ScreenPosition position) {
  auto box = obj->getBox();

  switch (position) {
    case ScreenPosition::TOP_LEFT:
      return obj->setPosition({});
    case ScreenPosition::TOP_CENTER:
      return obj->setPosition({(WINDOW_WIDTH / 2) - box.getWidth() / 2, 0});
    case ScreenPosition::TOP_RIGHT:
      return obj->setPosition({WINDOW_WIDTH - box.getWidth(), 0});
    case ScreenPosition::MIDDLE_LEFT:
      return obj->setPosition({0, (WINDOW_HEIGHT / 2) - (box.getHeight() / 2)});
    case ScreenPosition::MIDDLE_CENTER:
      return obj->setPosition({(WINDOW_WIDTH / 2) - (box.getWidth() / 2),
                               (WINDOW_HEIGHT / 2) - (box.getHeight() / 2)});
    case ScreenPosition::MIDDLE_RIGHT:
      return obj->setPosition({WINDOW_WIDTH - box.getWidth(),
                               (WINDOW_HEIGHT / 2) - (box.getHeight() / 2)});
    case ScreenPosition::BOTTOM_LEFT:
      return obj->setPosition({0, WINDOW_HEIGHT - box.getHeight()});
    case ScreenPosition::BOTTOM_CENTER:
      return obj->setPosition({(WINDOW_WIDTH / 2) - (box.getWidth() / 2),
                               WINDOW_HEIGHT - box.getHeight()});
    case ScreenPosition::BOTTOM_RIGHT:
      return obj->setPosition(
        {WINDOW_WIDTH - box.getWidth(), WINDOW_HEIGHT - box.getHeight()});
    default:
      return;
  }
}

void setPositionOnScreen(Object* obj, ScreenPosition::ScreenPosition position,
                         Vector offset) {
  setPositionOnScreen(obj, position);
  obj->setPosition(obj->getPosition() + offset);
}
}  // namespace lbui