#pragma once

#include "Manager.h"
#include "Object.h"
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
  // The object the view is following, if any
  Object *viewFollowing;

  // Move object and check if it is out of bounds
  void moveAndCheckBounds(Object *o, Vector where);

 public:
  static auto getInstance() -> WorldManager &;

  auto startUp() -> int override;
  void shutDown() override;

  // Adds an objects to the list of active objects
  auto insertObject(Object *o) -> int;

  // Removes on object from the list of active objects
  auto removeObject(Object *o) -> int;

  // Returns all active objects
  auto getAllObjects() const -> ObjectList;

  // Returns a list of all active objects of a given type
  auto objectsOfType(std::string type) const -> ObjectList;

  // Returns a list of object colliding with the object at a given position
  auto getCollisions(Object *o, Vector where) const -> ObjectList;

  // Attempts to move an object to a given position.
  // Returns 0 for success, -1 for failure
  auto moveObject(Object *o, Vector where) -> int;

  // Updates all active objects and frees resources for deleted ones
  void update();

  // Marks an object to be deleted in the next update call
  auto markForDelete(Object *o) -> int;

  // Draws all the active objects in the view
  void draw();

  // Set the boundary for the world
  void setBoundary(Box b);
  // Get the boundary for the world
  auto getBoundary() const -> Box;

  // Set the current view (i.e., visible portion of the world)
  void setView(Box v);
  // Get the current view
  auto getView() const -> Box;

  // Centers the view on a given position.
  // View edge will not go beyond world boundary.
  void setViewPosition(Vector where);

  // Set the view to follow a given object.
  // Set to NULL to stop following.
  // Returns 0 for success, -1 for failure
  auto setViewFollowing(Object *o) -> int;
};
}  // namespace df
