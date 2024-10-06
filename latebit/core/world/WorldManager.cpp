#include "WorldManager.h"

#include <cstdio>
#include <vector>

#include "latebit/core/events/EventCollision.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/Object.h"
#include "latebit/core/objects/ObjectUtils.h"
#include "latebit/core/utils/utils.h"
#include "latebit/core/world/View.h"
#include "latebit/utils/Logger.h"

#define WM lb::WorldManager::getInstance()

namespace lb {

WorldManager::WorldManager() : Manager("WorldManager") {
  Log.debug("WorldManager::WorldManager(): Created WorldManager");
}

auto WorldManager::getInstance() -> WorldManager & {
  static WorldManager instance;
  return instance;
}

auto WorldManager::startUp() -> int {
  this->sceneGraph.clear();
  this->view = View(this);
  Log.info("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  this->sceneGraph.clear();
  Manager::shutDown();
  Log.info("WorldManager::shutDown(): Shut down successfully");
}

auto WorldManager::isValid([[maybe_unused]] string eventType) const -> bool {
  // World Manager needs to accept all the events because it is the Manager
  // responsible for registering custom events
  return true;
}

auto WorldManager::getAllObjects(bool includeInactive) const
  -> vector<Object *> {
  if (includeInactive) {
    auto active = this->sceneGraph.getActiveObjects();
    auto inactive = this->sceneGraph.getInactiveObjects();
    active.insert(active.end(), inactive.begin(), inactive.end());
    return active;
  }

  return this->sceneGraph.getActiveObjects();
}

auto WorldManager::getAllObjectsByType(
  std::string type, bool includeInactive) const -> vector<Object *> {
  vector<Object *> result = {};
  auto all = this->getAllObjects(includeInactive);
  result.reserve(all.size());

  for (auto object : all) {
    if (object->getType() == type) {
      result.push_back(object);
    }
  }

  return result;
}

auto WorldManager::getCollisions(Object *o,
                                 Vector where) const -> vector<Object *> {
  vector<Object *> collisions = {};
  auto solid = this->sceneGraph.getSolidObjects();
  auto box = o->getWorldBox(where);
  collisions.reserve(solid.size());

  for (auto current : solid) {
    if (current == o) continue;
    auto currentBox = current->getWorldBox();

    if (intersects(box, currentBox)) {
      collisions.push_back(current);
    }
  }

  return collisions;
}

void bounce(Object *object, Object *otherObject) {
  // We assume same mass and completely elastic collision
  auto velocity = object->getVelocity();
  object->setVelocity(otherObject->getVelocity());
  otherObject->setVelocity(velocity);

  auto acceleration = object->getAcceleration();
  object->setAcceleration(otherObject->getAcceleration());
  otherObject->setAcceleration(acceleration);
}

void WorldManager::resolveMovement(Object *object, Vector position) {
  // Non-solid can always move, since they have no collisions
  if (!object->isSolid()) {
    return moveAndCheckBounds(object, position);
  }

  vector<Object *> collisions = getCollisions(object, position);

  // In absence of collisions, just move
  if (collisions.empty()) {
    return moveAndCheckBounds(object, position);
  }

  bool shouldMove = true;
  for (auto otherObject : collisions) {
    auto event = EventCollision(object, otherObject, position);
    object->eventHandler(&event);
    otherObject->eventHandler(&event);

    if (object->getSolidness() == Solidness::HARD &&
        otherObject->getSolidness() == Solidness::HARD) {
      bounce(object, otherObject);
      shouldMove = false;
      break;
    }
  }

  if (shouldMove) moveAndCheckBounds(object, position);
}

void WorldManager::moveAndCheckBounds(Object *o, Vector position) {
  auto initial = o->getWorldBox();
  o->setPosition(position);
  auto final = o->getWorldBox();
  auto boundary = WM.getBoundary();

  if (intersects(initial, boundary) && !intersects(final, boundary)) {
    auto event = EventOut();
    o->eventHandler(&event);
  }

  if (this->view.getViewFollowing() == o) {
    auto viewDeadZone = this->view.getViewDeadZone();

    // Move the view if the object is outside of the dead zone
    if (!contains(viewDeadZone, final)) {
      this->view.setViewPosition(position);
    }
  }
}

void WorldManager::update() {
  if (!isStarted()) return;

  // Delete objects marked for deletion
  for (auto &object : deletions) {
    this->sceneGraph.removeObject(object);
  }
  deletions.clear();

  auto active = this->sceneGraph.getActiveObjects();
  for (auto &object : active) {
    auto oldPosition = object->getPosition();
    auto newVelocity = object->getVelocity() + object->getAcceleration();
    auto newPosition = object->getPosition() + newVelocity;

    object->setVelocity(newVelocity);
    if (oldPosition != newPosition) {
      this->resolveMovement(object, newPosition);
    }
  }
}

auto WorldManager::markForDelete(Object *o) -> int {
  o->unsubscribeAll();
  o->teardown();
  insert(deletions, o);
  return 0;
}

void WorldManager::draw() {
  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    auto visible = this->sceneGraph.getVisibleObjects(i);
    for (auto &o : visible) {
      if (o != nullptr && intersects(o->getWorldBox(), this->view.getView())) {
        o->draw();
      };
    }
  }
}

void WorldManager::setBoundary(Box b) { this->boundary = b; }
auto WorldManager::getBoundary() const -> Box { return this->boundary; }

auto WorldManager::getSceneGraph() -> SceneGraph & { return this->sceneGraph; }
auto WorldManager::getView() -> View & { return this->view; }
}  // namespace lb
