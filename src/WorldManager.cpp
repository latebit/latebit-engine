#include "WorldManager.h"

#include "LogManager.h"
#include "ObjectListIterator.h"

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
  auto iterator = ObjectListIterator(&updates);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    delete iterator.currentObject();
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

void WorldManager::update() {
  auto i_deletions = ObjectListIterator(&m_deletions);
  for (i_deletions.first(); !i_deletions.isDone(); i_deletions.next()) {
    removeObject(i_deletions.currentObject());
    delete i_deletions.currentObject();
  }
  m_deletions.clear();

  auto i_updates = ObjectListIterator(&m_updates);
  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    auto object = i_updates.currentObject();
    auto old_position = object->getPosition();
    auto new_position = object->predictPosition();

    if (old_position != new_position) {
      // TODO: enrich with collision checks
      object->setPosition(new_position);
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
  auto iterator = new ObjectListIterator(&m_updates);

  for (int i = 0; i < MAX_ALTITUDE; i++) {
    for (iterator->first(); !iterator->isDone(); iterator->next()) {
      auto object = iterator->currentObject();

      if (object != nullptr && object->getAltitude() == i) object->draw();
    }
  }
}
}  // namespace df
