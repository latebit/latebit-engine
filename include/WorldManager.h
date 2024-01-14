#pragma once

#include "Manager.h"
#include "ObjectList.h"

#define WM df::WorldManager::getInstance()

namespace df {

const int MAX_ALTITUDE = 4;

class WorldManager : public Manager {
 private:
  // Make WorldManager a singleton
  WorldManager();
  WorldManager(WorldManager const &);
  void operator=(WorldManager const &);

  // Objects that receive the update callback
  ObjectList updates;
  // Objects that are marked for deletion
  ObjectList deletions;
  // The boundaries of the world, regardless of where the camera points
  Box boundary;
  // The boundaries of the visible portion of the world
  Box view;

  // Return true if a position is out side of the current view
  bool isOutOfBounds(Vector p) const;

  // Move object and check if it is out of bounds
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

  // Set the boundary for the world
  void setBoundary(Box b);
  // Get the boundary for the world
  Box getBoundary() const;

  // Set the current view (i.e., visible portion of the world)
  void setView(Box v);
  // Get the current view
  Box getView() const;
};
}  // namespace df
