#pragma once

#include "latebit/core/geometry/Box.h"
#include "latebit/core/objects/Object.h"

namespace lb {
class WorldManager;

class View {
 private:
  const WorldManager *world = nullptr;
  // The boundaries of the visible portion of the world in cells
  Box view = Box();
  // The object the view is following, if any
  Object *viewFollowing = nullptr;
  // Part of the view where viewFollowing can move without moving the camera
  Box viewDeadZone = Box();

 public:
  explicit View(const WorldManager *world) : world(world) {};

  // Set the current view (i.e., visible portion of the world)
  void setView(Box v);
  // Get the current view
  [[nodiscard]] auto getView() const -> Box;

  // Centers the view on a given position.
  // View edge will not go beyond world boundary.
  void setViewPosition(Vector where);

  // Set the view to follow a given object.
  // Set to NULL to stop following.
  // Returns 0 for success, -1 for failure
  auto setViewFollowing(Object *o) -> int;

  auto getViewFollowing() -> Object *;

  // Set the dead zone for the view following
  void setViewDeadZone(Box d);
  // Get the dead zone for the view following
  [[nodiscard]] auto getViewDeadZone() const -> Box;

  auto worldToView(Vector worldPosition) -> Vector;

  auto viewToWorld(Vector viewPosition) -> Vector;
};
}  // namespace lb