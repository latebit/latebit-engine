#include "SceneGraph.h"

#include <algorithm>
#include <memory>

#include "latebit/core/objects/Object.h"
#include "latebit/core/objects/ObjectUtils.h"

namespace lb {
SceneGraph::SceneGraph() {
  all.reserve(100);
  active.reserve(100);
  inactive.reserve(100);
  solid.reserve(100);
  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    visible[i].reserve(100);
  }
};

auto SceneGraph::insertObject(unique_ptr<Object> o) -> int {
  auto rawO = o.get();

  if (o->isActive()) {
    insert(active, rawO);
  } else {
    insert(inactive, rawO);
  }

  if (o->isSolid()) {
    insert(solid, rawO);
  }

  if (o->isVisible()) {
    insert(visible.at(o->getAltitude()), rawO);
  }

  insert(all, std::move(o));
  return 0;
}

auto SceneGraph::removeObject(Object *rawO) -> int {
  if (!rawO) return 0;

  if (rawO->isActive()) {
    remove(active, rawO);
  } else {
    remove(inactive, rawO);
  }

  if (rawO->isSolid()) {
    remove(solid, rawO);
  }

  if (rawO->isVisible()) {
    auto& visible = this->visible.at(rawO->getAltitude());
    remove(visible, rawO);
  }

  all.erase(std::remove_if(
    all.begin(), all.end(),
    [rawO](const std::unique_ptr<Object> &p) { return p.get() == rawO; }),
  all.end());
  return 0;
}

auto SceneGraph::getActiveObjects() const -> vector<Object*> { return this->active; }

auto SceneGraph::getInactiveObjects() const -> vector<Object*> {
  return this->inactive;
}

auto SceneGraph::getSolidObjects() const -> vector<Object*> { return this->solid; }

auto SceneGraph::getVisibleObjects(int altitude) const -> vector<Object*> {
  if (altitude < 0 || altitude > MAX_ALTITUDE) {
    return {};
  }

  return visible.at(altitude);
}

auto SceneGraph::setSolidness(Object *rawO,
                              Solidness::Solidness solidness) -> int {
  bool isSolid = solidness == Solidness::HARD || solidness == Solidness::SOFT;

  if (rawO->isSolid()) remove(solid, rawO);

  if (isSolid) insert(solid, rawO);
  
  return 0;
}

auto SceneGraph::setAltitude(Object *rawO, int altitude) -> int {
  if (altitude < 0 || altitude > MAX_ALTITUDE) {
    return -1;
  }

  remove(visible.at(rawO->getAltitude()), rawO);
  insert(visible.at(altitude), rawO);
  return 0;
}

auto SceneGraph::setActive(Object *rawO, bool isActive) -> int {
  if (rawO->isActive() == isActive) {
    return 0;
  }

  // Switching from active to inactive
  if (rawO->isActive()) {
    remove(active, rawO);
    insert(inactive, rawO);

    auto& visible = this->visible.at(rawO->getAltitude());
    remove(visible, rawO);

    if (rawO->isSolid()) remove(solid, rawO);

    return 0;
  }

  // Switching from inactive to active
  insert(active, rawO);
  remove(inactive, rawO);
  
  auto& visible = this->visible.at(rawO->getAltitude());
  insert(visible, rawO);

  if (rawO->isSolid()) insert(solid, rawO);
  
  return 0;
}

auto SceneGraph::setVisible(Object *rawO, bool isVisible) -> int {
  if (rawO->isVisible() == isVisible) return 0;

  auto altitude = rawO->getAltitude();
  
  if (!isVisible) {
    remove(visible.at(altitude), rawO);
    return 0;
  }

  insert(visible.at(altitude), rawO);
  return 0;
}

auto SceneGraph::getAllObjects() const -> const vector<unique_ptr<Object>>& {
  return this->all;
}

auto SceneGraph::clear() -> void {
  all.clear();
  active.clear();
  inactive.clear();
  solid.clear();
  for (auto& v : visible) {
    v.clear();
  }
}

}  // namespace lb
