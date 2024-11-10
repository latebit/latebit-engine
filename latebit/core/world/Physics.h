#pragma once

#include "latebit/core/world/Object.h"
#include "latebit/core/world/SceneGraph.h"

namespace lb {
class WorldManager;

class Physics {
 private:
  const SceneGraph* sceneGraph = nullptr;
  const WorldManager *world = nullptr;

 public:
  Physics(SceneGraph* graph, WorldManager* world) : sceneGraph(graph), world(world) {};
  ~Physics() = default;

  // Update the physics of the world
  void update();

  // Get all objects that overlap with the object at the given position
  [[nodiscard]] auto getOverlapsAt(Object*, Vector) const -> vector<Object*>;

  // Move an object to a new position skipping any physics simulation.
  // If the object is moved out of bounds, an EventOut is emitted.
  void moveTo(Object*, Vector);
};
}  // namespace lb