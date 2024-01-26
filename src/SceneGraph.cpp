#include "SceneGraph.h"

#include "Object.h"
#include "ObjectList.h"

namespace df {
SceneGraph::SceneGraph() = default;
SceneGraph::~SceneGraph() = default;

auto SceneGraph::insertObject(Object *o) -> int {
  if (this->all.insert(o) != 0) {
    return -1;
  }

  if (o->isSolid()) {
    if (this->solid.insert(o) != 0) {
      return -1;
    }
  }

  if (this->visible.at(o->getAltitude()).insert(o) != 0) {
    return -1;
  }

  return 0;
}

auto SceneGraph::removeObject(Object *o) -> int {
  int result = this->all.remove(o);

  if (o->isSolid()) {
    result += this->solid.remove(o);
  }

  result += this->visible.at(o->getAltitude()).remove(o);
  return result;
}

auto SceneGraph::getAllObjects() const -> ObjectList { return this->all; }

auto SceneGraph::getSolidObjects() const -> ObjectList { return this->solid; }

auto SceneGraph::getVisibleObjects(int altitude) const -> ObjectList {
  if (altitude < 0 || altitude > MAX_ALTITUDE) {
    return {};
  }

  return visible.at(altitude);
}

auto SceneGraph::setSolidness(Object *o, Solidness solidness) -> int {
  if (o->isSolid()) {
    if (this->solid.remove(o) != 0) {
      return -1;
    }
  }

  if (solidness == HARD || solidness == SOFT) {
    if (this->solid.insert(o) != 0) {
      return -1;
    }
  }

  return 0;
}

auto SceneGraph::setAltitude(Object *o, int altitude) -> int {
  if (altitude < 0 || altitude > MAX_ALTITUDE) {
    return -1;
  }

  if (this->visible.at(o->getAltitude()).remove(o) != 0) {
    return -1;
  }

  if (this->visible.at(altitude).insert(o) != 0) {
    return -1;
  }

  return 0;
}

}  // namespace df
