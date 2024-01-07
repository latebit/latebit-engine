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
  m_deletions = ObjectList();
  m_updates = ObjectList();
  LM.writeLog("WorldManager::WorldManager(): Created WorldManager");
}

WorldManager& WorldManager::getInstance() {
  static WorldManager instance;
  return instance;
}

int WorldManager::startUp() {
  m_deletions = ObjectList();
  m_updates = ObjectList();
  LM.writeLog("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  auto updates = m_updates;  // create a copy
  auto i_updates = ObjectListIterator(&updates);
  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    delete i_updates.currentObject();
  }

  m_updates.clear();

  Manager::shutDown();
  LM.writeLog("WorldManager::shutDown(): Shut down successfully");
}

int WorldManager::insertObject(Object* p_o) { return m_updates.insert(p_o); }

int WorldManager::removeObject(Object* p_o) { return m_updates.remove(p_o); }

ObjectList WorldManager::getAllObjects() const { return m_updates; }

ObjectList WorldManager::objectsOfType(std::string type) const {
  ObjectList result;
  auto i_updates = ObjectListIterator(&m_updates);

  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    if (i_updates.currentObject()->getType() == type) {
      result.insert(i_updates.currentObject());
    }
  }

  return result;
}

ObjectList WorldManager::getCollisions(Object* p_o, Vector where) const {
  ObjectList collisions;
  auto i_updates = ObjectListIterator(&m_updates);
  auto box = p_o->getWorldBox(where);

  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    auto p_current = i_updates.currentObject();
    if (p_current == p_o) continue;

    auto currentBox = p_current->getWorldBox();

    if (p_current->isSolid() && intersects(box, currentBox)) {
      collisions.insert(p_current);
    }
  }

  return collisions;
}

int WorldManager::moveObject(Object* p_o, Vector where) {
  // Spectral objects can just move
  if (!p_o->isSolid()) {
    moveAndCheckBounds(p_o, where);
    return 0;
  }

  ObjectList collisions = getCollisions(p_o, where);

  // In absence of collisions, just move
  if (collisions.isEmpty()) {
    moveAndCheckBounds(p_o, where);
    return 0;
  }

  bool shouldMove = true;
  auto i_collisions = ObjectListIterator(&collisions);

  for (i_collisions.first(); !i_collisions.isDone(); i_collisions.next()) {
    auto p_current = i_collisions.currentObject();
    auto event = EventCollision(p_o, p_current, where);
    p_o->eventHandler(&event);
    p_current->eventHandler(&event);

    // If hitting a hard object, don't move
    if (p_o->getSolidness() == HARD && p_current->getSolidness() == HARD) {
      shouldMove = false;
      break;
    }
  }

  if (shouldMove) {
    moveAndCheckBounds(p_o, where);
    return 0;
  } else {
    return -1;
  }

  return 0;
}

void WorldManager::moveAndCheckBounds(Object* p_o, Vector where) const {
  auto old_position = p_o->getPosition();
  p_o->setPosition(where);

  if (isOutOfBounds(where) && !isOutOfBounds(old_position)) {
    auto event = EventOut();
    p_o->eventHandler(&event);
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

  auto i_deletions = ObjectListIterator(&m_deletions);
  for (i_deletions.first(); !i_deletions.isDone(); i_deletions.next()) {
    removeObject(i_deletions.currentObject());
    delete i_deletions.currentObject();
  }
  m_deletions.clear();

  auto i_updates = ObjectListIterator(&m_updates);
  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    auto object = i_updates.currentObject();
    auto oldPosition = object->getPosition();
    auto newPosition = object->predictPosition();

    if (oldPosition != newPosition) {
      moveObject(object, newPosition);
    }
  }
}

int WorldManager::markForDelete(Object* p_o) {
  // Prevents marking the same object for deletion twice
  auto i_deletions = ObjectListIterator(&m_deletions);
  for (i_deletions.first(); !i_deletions.isDone(); i_deletions.next()) {
    if (i_deletions.currentObject() == p_o) return 0;
  }

  return m_deletions.insert(p_o);
}

void WorldManager::draw() {
  auto i_updates = new ObjectListIterator(&m_updates);

  for (int i = 0; i < MAX_ALTITUDE; i++) {
    for (i_updates->first(); !i_updates->isDone(); i_updates->next()) {
      auto object = i_updates->currentObject();

      if (object != nullptr && object->getAltitude() == i) object->draw();
    }
  }
}
}  // namespace df
