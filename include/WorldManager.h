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

  // Move object and check if it is out of bounds
  void moveAndCheckBounds(Object *o, Vector where) const;

 public:
  static auto getInstance() -> WorldManager &;

  auto startUp() -> int override;
  void shutDown() override;
  auto insertObject(Object *o) -> int;
  auto removeObject(Object *o) -> int;
  auto getAllObjects() const -> ObjectList;
  auto objectsOfType(std::string type) const -> ObjectList;

  auto getCollisions(Object *o, Vector where) const -> ObjectList;
  auto moveObject(Object *o, Vector where) -> int;

  void update();
  auto markForDelete(Object *o) -> int;
  void draw();

  // Set the boundary for the world
  void setBoundary(Box b);
  // Get the boundary for the world
  auto getBoundary() const -> Box;

  // Set the current view (i.e., visible portion of the world)
  void setView(Box v);
  // Get the current view
  auto getView() const -> Box;
};
}  // namespace df
