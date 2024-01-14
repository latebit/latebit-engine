#include "WorldManager.h"

#include "DisplayManager.h"
#include "EventCollision.h"
#include "EventOut.h"
#include "LogManager.h"
#include "ObjectListIterator.h"
#include "utils.h"

#define WM df::WorldManager::getInstance()

namespace df {

WorldManager::WorldManager() {
  setType("WorldManager");
  this->deletions = ObjectList();
  this->updates = ObjectList();
  LM.writeLog("WorldManager::WorldManager(): Created WorldManager");
}

WorldManager& WorldManager::getInstance() {
  static WorldManager instance;
  return instance;
}

int WorldManager::startUp() {
  this->deletions = ObjectList();
  this->updates = ObjectList();
  LM.writeLog("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  auto updates = this->updates;  // create a copy
  auto iterator = ObjectListIterator(&updates);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    delete iterator.currentObject();
  }

  this->updates.clear();

  Manager::shutDown();
  LM.writeLog("WorldManager::shutDown(): Shut down successfully");
}

int WorldManager::insertObject(Object* o) { return this->updates.insert(o); }

int WorldManager::removeObject(Object* o) { return this->updates.remove(o); }

ObjectList WorldManager::getAllObjects() const { return this->updates; }

ObjectList WorldManager::objectsOfType(std::string type) const {
  ObjectList result;
  auto iterator = ObjectListIterator(&this->updates);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject()->getType() == type) {
      result.insert(iterator.currentObject());
    }
  }

  return result;
}

ObjectList WorldManager::getCollisions(Object* o, Vector where) const {
  ObjectList collisions;
  auto iterator = ObjectListIterator(&this->updates);
  auto box = o->getWorldBox(where);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    auto current = iterator.currentObject();
    if (current == o) continue;

    auto currentBox = current->getWorldBox();

    if (current->isSolid() && intersects(box, currentBox)) {
      collisions.insert(current);
    }
  }

  return collisions;
}

int WorldManager::moveObject(Object* o, Vector where) {
  // Spectral objects can just move
  if (!o->isSolid()) {
    moveAndCheckBounds(o, where);
    return 0;
  }

  ObjectList collisions = getCollisions(o, where);

  // In absence of collisions, just move
  if (collisions.isEmpty()) {
    moveAndCheckBounds(o, where);
    return 0;
  }

  bool shouldMove = true;
  auto iterator = ObjectListIterator(&collisions);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    auto current = iterator.currentObject();
    auto event = EventCollision(o, current, where);
    o->eventHandler(&event);
    current->eventHandler(&event);

    // If hitting a hard object, don't move
    if (o->getSolidness() == HARD && current->getSolidness() == HARD) {
      shouldMove = false;
      break;
    }
  }

  if (shouldMove) {
    moveAndCheckBounds(o, where);
    return 0;
  } else {
    return -1;
  }

  return 0;
}

void WorldManager::moveAndCheckBounds(Object* o, Vector where) const {
  auto old_position = o->getPosition();
  o->setPosition(where);

  // Should this take in account the bounding box?
  if (isOutOfBounds(where) && !isOutOfBounds(old_position)) {
    auto event = EventOut();
    o->eventHandler(&event);
  }
}

bool WorldManager::isOutOfBounds(Vector where) const {
  auto x = where.getX();
  auto y = where.getY();

  return (x < 0 || y < 0 || x >= DM.getHorizontalCells() ||
          y >= DM.getVerticalCells());
}

void WorldManager::update() {
  if (!isStarted()) return;

  auto deletions = ObjectListIterator(&this->deletions);
  for (deletions.first(); !deletions.isDone(); deletions.next()) {
    removeObject(deletions.currentObject());
    delete deletions.currentObject();
  }
  this->deletions.clear();

  auto updates = ObjectListIterator(&this->updates);
  for (updates.first(); !updates.isDone(); updates.next()) {
    auto object = updates.currentObject();
    auto oldPosition = object->getPosition();
    auto newPosition = object->predictPosition();

    if (oldPosition != newPosition) {
      moveObject(object, newPosition);
    }
  }
}

int WorldManager::markForDelete(Object* o) {
  // Prevents marking the same object for deletion twice
  auto iterator = ObjectListIterator(&this->deletions);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject() == o) return 0;
  }

  return this->deletions.insert(o);
}

void WorldManager::draw() {
  auto iterator = new ObjectListIterator(&this->updates);

  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    for (iterator->first(); !iterator->isDone(); iterator->next()) {
      auto object = iterator->currentObject();

      if (object != nullptr && object->getAltitude() == i) object->draw();
    }
  }
}
}  // namespace df
