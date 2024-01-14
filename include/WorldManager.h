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

  ObjectList updates;
  ObjectList deletions;

  bool isOutOfBounds(Vector p) const;
  void moveAndCheckBounds(Object *o, Vector where) const;

 public:
  static WorldManager &getInstance();

  int startUp();
  void shutDown();
  int insertObject(Object *o);
  int removeObject(Object *o);
  ObjectList getAllObjects() const;
  ObjectList objectsOfType(std::string type) const;

  ObjectList getCollisions(Object *o, Vector where) const;
  int moveObject(Object *o, Vector where);

  void update();
  int markForDelete(Object *o);
  void draw();
};
}  // namespace df
