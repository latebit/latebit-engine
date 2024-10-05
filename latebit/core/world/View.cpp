#include "View.h"
#include "latebit/core/utils/utils.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Math.h"
#include "latebit/core/world/WorldManager.h"

namespace lb {

void View::setView(Box v) { this->view = v; }
auto View::getView() const -> Box { return this->view; }

auto View::setViewPosition(Vector where) -> void {
  auto viewHeight = this->view.getHeight();
  auto viewWidth = this->view.getWidth();
  auto boundary = this->world->getBoundary();
  auto worldHeight = boundary.getHeight();
  auto worldWidth = boundary.getWidth();
  auto worldOrigin = boundary.getCorner();
  auto worldOriginX = worldOrigin.getX();
  auto worldOriginY = worldOrigin.getY();

  // easier to understand as the following translations
  // viewCenter = corner + Vector(width / 2, height / 2)
  // translate = where - viewCenter (note, `where` is the center of the view)
  // newCorner = viewCorner + translate
  auto newCorner = where - Vector(viewWidth / 2, viewHeight / 2);

  newCorner.setX(clamp(newCorner.getX(), worldOriginX,
                       worldOriginX + worldWidth - viewWidth));
  newCorner.setY(clamp(newCorner.getY(), worldOriginY,
                       worldOriginY + worldHeight - viewHeight));

  this->view = Box(newCorner, viewWidth, viewHeight);
}

auto View::setViewFollowing(Object* o) -> int {
  if (o == nullptr) {
    this->viewFollowing = nullptr;
    return 0;
  }

  auto updates = world->getAllObjects();
  for (auto object : updates) {
    if (object == o) {
      this->viewFollowing = o;
      return 0;
    }
  }

  Log.error(
    "View::setViewFollowing(): Object %s to be followed was not found",
    o->toString().c_str());
  return -1;
}

auto View::getViewFollowing() -> Object* { return this->viewFollowing; }

void View::setViewDeadZone(Box d) {
  if (!contains(this->view, d)) {
    Log.error("View::setViewDeadZone(): Dead zone larger than view");
    return;
  }

  this->viewDeadZone = d;
}

auto View::getViewDeadZone() const -> Box { return this->viewDeadZone; }

auto View::worldToView(Vector worldPosition) -> Vector {
  auto viewOrigin = this->view.getCorner();
  return worldPosition - viewOrigin;
}

auto View::viewToWorld(Vector viewPosition) -> Vector {
  auto viewOrigin = this->view.getCorner();
  return viewPosition + viewOrigin;
}
};


