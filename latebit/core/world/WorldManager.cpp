#include "WorldManager.h"
#include <cstdio>

#include "latebit/core/events/EventCollision.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/Object.h"
#include "latebit/core/objects/ObjectListIterator.h"
#include "latebit/core/utils/utils.h"
#include "latebit/core/world/View.h"
#include "latebit/utils/Logger.h"

#define WM lb::WorldManager::getInstance()

namespace lb {

WorldManager::WorldManager() : Manager("WorldManager") {
  Log.debug("WorldManager::WorldManager(): Created WorldManager");
}

auto WorldManager::getInstance() -> WorldManager& {
  static WorldManager instance;
  return instance;
}

auto WorldManager::startUp() -> int {
  this->deletions = ObjectList();
  this->sceneGraph = SceneGraph();
  this->view = View(this);
  Log.info("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  auto all = this->getAllObjects(true);
  auto iterator = ObjectListIterator(&all);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    auto current = iterator.currentObject();
    delete current;
    current = nullptr;
  }

  Manager::shutDown();
  Log.info("WorldManager::shutDown(): Shut down successfully");
}

auto WorldManager::isValid([[maybe_unused]] string eventType) const -> bool {
  // World Manager needs to accept all the events because it is the Manager
  // responsible for registering custom events
  return true;
}

auto WorldManager::insertObject(Object* o) -> int {
  return this->sceneGraph.insertObject(o);
}

auto WorldManager::removeObject(Object* o) -> int {
  return this->sceneGraph.removeObject(o);
}

auto WorldManager::getAllObjects(bool includeInactive) const -> ObjectList {
  if (includeInactive) {
    return this->sceneGraph.getActiveObjects() +
           this->sceneGraph.getInactiveObjects();
  }

  return this->sceneGraph.getActiveObjects();
}

auto WorldManager::getAllObjectsByType(std::string type,
                                 bool includeInactive) const -> ObjectList {
  ObjectList result;
  auto solid = this->getAllObjects(includeInactive);
  auto iterator = ObjectListIterator(&solid);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject()->getType() == type) {
      result.insert(iterator.currentObject());
    }
  }

  return result;
}

auto WorldManager::getCollisions(Object* o, Vector where) const -> ObjectList {
  ObjectList collisions;
  auto solid = this->sceneGraph.getSolidObjects();
  auto iterator = ObjectListIterator(&solid);
  auto box = o->getWorldBox(where);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    auto current = iterator.currentObject();
    if (current == o) continue;

    auto currentBox = current->getWorldBox();

    if (intersects(box, currentBox)) {
      collisions.insert(current);
    }
  }

  return collisions;
}

void bounce(Object* object, Object *otherObject) {
  // We assume same mass and completely elastic collision
  auto velocity = object->getVelocity();
  object->setVelocity(otherObject->getVelocity());
  otherObject->setVelocity(velocity);

  auto acceleration = object->getAcceleration();
  object->setAcceleration(otherObject->getAcceleration());
  otherObject->setAcceleration(acceleration);
}

void WorldManager::resolveMovement(Object* object, Vector position) {
  // Non-solid can always move, since they have no collisions
  if (!object->isSolid()) {
    return moveAndCheckBounds(object, position);
  }

  ObjectList collisions = getCollisions(object, position);

  // In absence of collisions, just move
  if (collisions.isEmpty()) {
    return moveAndCheckBounds(object, position);
  }

  auto iterator = ObjectListIterator(&collisions);

  bool shouldMove = true;
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    auto otherObject = iterator.currentObject();
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

void WorldManager::moveAndCheckBounds(Object* o, Vector position) {
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

  auto deletions = ObjectListIterator(&this->deletions);
  for (deletions.first(); !deletions.isDone(); deletions.next()) {
    Log.debug("WorldManager::update(): Deleting object %s",
              deletions.currentObject()->toString().c_str());
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    auto current = deletions.currentObject();
    delete current;
    current = nullptr;
  }
  this->deletions.clear();

  auto active = this->sceneGraph.getActiveObjects();
  auto updates = ObjectListIterator(&active);
  for (updates.first(); !updates.isDone(); updates.next()) {
    auto object = updates.currentObject();
    auto oldPosition = object->getPosition();
    auto newVelocity = object->getVelocity() + object->getAcceleration();
    auto newPosition = object->getPosition() + newVelocity;

    object->setVelocity(newVelocity);
    if (oldPosition != newPosition) {
      this->resolveMovement(object, newPosition);
    }
  }
}

auto WorldManager::markForDelete(Object* o) -> int {
  // Prevents marking the same object for deletion twice
  auto iterator = ObjectListIterator(&this->deletions);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject() == o) return 0;
  }

  return this->deletions.insert(o);
}

void WorldManager::draw() {
  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    auto visible = this->getSceneGraph().getVisibleObjects(i);
    auto iterator = ObjectListIterator(&visible);

    for (iterator.first(); !iterator.isDone(); iterator.next()) {
      auto object = iterator.currentObject();
      if (object != nullptr && intersects(object->getWorldBox(), this->view.getView())) {
        object->draw();
      };
    }
  }
}

void WorldManager::setBoundary(Box b) { this->boundary = b; }
auto WorldManager::getBoundary() const -> Box { return this->boundary; }

auto WorldManager::getSceneGraph() -> SceneGraph& { return this->sceneGraph; }
auto WorldManager::getView() -> View& { return this->view; }
}  // namespace lb
