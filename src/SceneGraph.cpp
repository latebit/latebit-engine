#include "SceneGraph.h"

#include "Logger.h"
#include "Object.h"
#include "ObjectList.h"

namespace lb {
SceneGraph::SceneGraph() = default;
SceneGraph::~SceneGraph() = default;

auto SceneGraph::insertObject(Object *o) -> int {
  if (o->isActive()) {
    if (this->active.insert(o) != 0) {
      Log.warning("SceneGraph::insertObject(): Failed to insert", o->toString(),
                  "in active list");
      return -1;
    }
  } else {
    if (this->inactive.insert(o) != 0) {
      Log.warning("SceneGraph::insertObject(): Failed to insert", o->toString(),
                  "in inactive list");
      return -1;
    }
  }

  if (o->isSolid()) {
    if (this->solid.insert(o) != 0) {
      Log.warning("SceneGraph::insertObject(): Failed to insert", o->toString(),
                  "in solid list");
      return -1;
    }
  }

  if (o->isVisible()) {
    if (this->visible.at(o->getAltitude()).insert(o) != 0) {
      Log.warning("SceneGraph::insertObject(): Failed to insert", o->toString(),
                  "in visible list. Altitude ", o->getAltitude());
      return -1;
    }
  }

  return 0;
}

auto SceneGraph::removeObject(Object *o) -> int {
  if (o->isActive()) {
    if (this->active.remove(o) != 0) {
      Log.warning("SceneGraph::removeObject(): Failed to remove", o->toString(),
                  "from active list");
      return -1;
    }
  } else {
    if (this->inactive.remove(o) != 0) {
      Log.warning("SceneGraph::removeObject(): Failed to remove", o->toString(),
                  "from inactive list");
      return -1;
    }
  }

  if (o->isSolid()) {
    if (this->solid.remove(o) != 0) {
      Log.warning("SceneGraph::removeObject(): Failed to remove", o->toString(),
                  "from solid list");
      return -1;
    }
  }

  if (o->isVisible()) {
    if (this->visible.at(o->getAltitude()).remove(o) != 0) {
      Log.warning("SceneGraph::removeObject(): Failed to remove", o->toString(),
                  "from visible list. Altitude", o->getAltitude());
      return -1;
    }
  }

  return 0;
}

auto SceneGraph::getActiveObjects() const -> ObjectList { return this->active; }

auto SceneGraph::getInactiveObjects() const -> ObjectList {
  return this->inactive;
}

auto SceneGraph::getSolidObjects() const -> ObjectList { return this->solid; }

auto SceneGraph::getVisibleObjects(int altitude) const -> ObjectList {
  if (altitude < 0 || altitude > MAX_ALTITUDE) {
    return {};
  }

  return visible.at(altitude);
}

auto SceneGraph::setSolidness(Object *o, Solidness solidness) -> int {
  bool isSolid = solidness == HARD || solidness == SOFT;

  if (isSolid == o->isSolid()) {
    Log.debug("SceneGraph::setSolidness(): Object", o->toString(),
              "has already expected solidness");
    return 0;
  }

  if (o->isSolid()) {
    if (this->solid.remove(o) != 0) {
      Log.warning("SceneGraph::setSolidness(): Failed to remove", o->toString(),
                  "from solid list");
      return -1;
    }
  }

  if (isSolid) {
    if (this->solid.insert(o) != 0) {
      Log.warning("SceneGraph::setSolidness(): Failed to insert", o->toString(),
                  "in solid list");
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
    Log.warning("SceneGraph::setAltitude(): Failed to remove", o->toString(),
                "from visible list. Altitude", o->getAltitude());
    return -1;
  }

  if (this->visible.at(altitude).insert(o) != 0) {
    Log.warning("SceneGraph::setAltitude(): Failed to insert", o->toString(),
                "in visible list. Altitude", o->getAltitude());
    return -1;
  }

  return 0;
}

auto SceneGraph::setActive(Object *o, bool isActive) -> int {
  if (o->isActive() == isActive) {
    return 0;
  }

  // Switching from active to inactive
  if (o->isActive()) {
    if (this->active.remove(o) != 0) {
      Log.warning("SceneGraph::setActive(): Failed to remove", o->toString(),
                  "from active list");
      return -1;
    }

    if (this->inactive.insert(o) != 0) {
      Log.warning("SceneGraph::setActive(): Failed to insert", o->toString(),
                  "in inactive list");
      return -1;
    }

    if (this->visible[o->getAltitude()].remove(o) != 0) {
      Log.warning("SceneGraph::setActive(): Failed to remove", o->toString(),
                  "from visible list. Altitude", o->getAltitude());
      return -1;
    }

    if (o->isSolid() && this->solid.remove(o) != 0) {
      Log.warning("SceneGraph::setActive(): Failed to remove", o->toString(),
                  "from solid list");
      return -1;
    }

    return 0;
  }

  // Switching from inactive to active
  if (this->active.insert(o) != 0) {
    Log.warning("SceneGraph::setActive(): Failed to insert", o->toString(),
                "in active list");
    return -1;
  }

  if (this->inactive.remove(o) != 0) {
    Log.warning("SceneGraph::setActive(): Failed to remove", o->toString(),
                "from inactive list");
    return -1;
  }

  if (this->visible[o->getAltitude()].insert(o) != 0) {
    Log.warning("SceneGraph::setActive(): Failed to insert", o->toString(),
                "in visible list. Altitude", o->getAltitude());
    return -1;
  }

  if (o->isSolid() && this->solid.insert(o) != 0) {
    Log.warning("SceneGraph::setActive(): Failed to remove", o->toString(),
                "from solid list.");
    return -1;
  }

  return 0;
}

auto SceneGraph::setVisible(Object *o, bool isVisible) -> int {
  if (o->isVisible() == isVisible) {
    return 0;
  }

  if (o->isVisible()) {
    return this->visible[o->getAltitude()].remove(o);
  }

  return this->visible[o->getAltitude()].insert(o);
}

}  // namespace lb
