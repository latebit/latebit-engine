#pragma once

#include "Manager.h"
#include "ObjectList.h"

#define WM df::WorldManager::getInstance()

namespace df {

const int MAX_ALTITUDE = 4;

class WorldManager : public Manager {
 private:
  WorldManager();
  WorldManager(WorldManager const &);
  void operator=(WorldManager const &);

  ObjectList m_updates;
  ObjectList m_deletions;

  bool isOutOfBounds(Vector p) const;
  void moveAndCheckBounds(Object *p_o, Vector where) const;

 public:
  static WorldManager &getInstance();

  int startUp();
  void shutDown();
  int insertObject(Object *p_o);
  int removeObject(Object *p_o);
  ObjectList getAllObjects() const;
  ObjectList objectsOfType(std::string type) const;

  ObjectList getCollisions(Object *p_o, Vector where) const;
  int moveObject(Object *p_o, Vector where);

  void update();
  int markForDelete(Object *p_o);
  void draw();
};
}  // namespace df

