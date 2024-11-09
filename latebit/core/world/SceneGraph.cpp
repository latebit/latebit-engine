#include "SceneGraph.h"

#include "latebit/core/world/Object.h"
#include "latebit/core/world/ObjectUtils.h"

namespace lb {
SceneGraph::SceneGraph() {
  active.reserve(100);
  inactive.reserve(100);
  solid.reserve(100);
  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    visible[i].reserve(100);
  }
};

auto SceneGraph::insertObject(Object *rawO) -> int {
  if (rawO->isActive()) {
    insert(active, rawO);
  } else {
    insert(inactive, rawO);
  }

  if (rawO->isSolid()) {
    insert(solid, rawO);
  }

  if (rawO->isVisible()) {
    insert(visible.at(rawO->getAltitude()), rawO);
  }

  return 0;
}

auto SceneGraph::removeObject(Object *rawO) -> int {
  if (rawO->isActive()) {
    remove(active, rawO);
  } else {
    remove(inactive, rawO);
  }

  if (rawO->isSolid()) {
    remove(solid, rawO);
  }

  if (rawO->isVisible()) {
    auto &visible = this->visible.at(rawO->getAltitude());
    remove(visible, rawO);
  }

  return 0;
}

auto SceneGraph::getActiveObjects() const -> vector<Object *> {
  return this->active;
}

auto SceneGraph::getInactiveObjects() const -> vector<Object *> {
  return this->inactive;
}

auto SceneGraph::getSolidObjects() const -> vector<Object *> {
  return this->solid;
}

auto SceneGraph::getVisibleObjects(int altitude) const -> vector<Object *> {
  if (altitude < 0 || altitude > MAX_ALTITUDE) {
    return {};
  }

  return visible.at(altitude);
}

void SceneGraph::setBodyType(Object *rawO, BodyType type) {
  bool isSolid = type != BodyType::KINEMATIC;

  if (rawO->isSolid()) remove(solid, rawO);

  if (isSolid) insert(solid, rawO);
}

void SceneGraph::setAltitude(Object *rawO, int altitude) {
  if (altitude < 0 || altitude > MAX_ALTITUDE) return;

  remove(visible.at(rawO->getAltitude()), rawO);
  insert(visible.at(altitude), rawO);
}

void SceneGraph::setActive(Object *rawO, bool isActive){
  if (rawO->isActive() == isActive) return;

  // Switching from active to inactive
  if (rawO->isActive()) {
    remove(active, rawO);
    insert(inactive, rawO);
    if (rawO->isSolid()) remove(solid, rawO);

    return;
  }

  // Switching from inactive to active
  insert(active, rawO);
  remove(inactive, rawO);

  if (rawO->isSolid()) insert(solid, rawO);
}

void SceneGraph::setVisible(Object *rawO, bool isVisible){
  if (rawO->isVisible() == isVisible) return;

  auto altitude = rawO->getAltitude();

  if (!isVisible) {
    remove(visible.at(altitude), rawO);
    return;
  }

  insert(visible.at(altitude), rawO);
}

auto SceneGraph::clear() -> void {
  active.clear();
  inactive.clear();
  solid.clear();
  for (auto &v : visible) {
    v.clear();
  }
}

}  // namespace lb
