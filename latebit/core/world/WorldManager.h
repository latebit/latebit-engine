#pragma once

#include <unordered_set>
#include <vector>

#include "SceneGraph.h"
#include "View.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/Object.h"
#include "latebit/core/utils/Manager.h"

#define WM lb::WorldManager::getInstance()

using namespace std;

namespace lb {

class View;

class WorldManager : public Manager {
 private:
  // Make WorldManager a singleton
  WorldManager();

  unordered_set<Object *> deletions = {};
  // The boundaries of the world, regardless of where the camera points in cells
  Box boundary = Box();
  // The current SceneGraph
  SceneGraph sceneGraph = SceneGraph();
  // The current View
  View view = View(this);

  // Move object and check if it is out of bounds
  void moveAndCheckBounds(Object *o, Vector position);

 public:
  // Singleton
  WorldManager(WorldManager const &) = delete;
  void operator=(WorldManager const &) = delete;
  static auto getInstance() -> WorldManager &;

  auto startUp() -> int override;
  void shutDown() override;
  [[nodiscard]] auto isValid(string eventType) const -> bool override;

  // Returns all active objects
  [[nodiscard]] auto getAllObjects(bool includeInactive = false) const
    -> vector<Object *>;

  // Returns a list of all active objects of a given type
  [[nodiscard]] auto getAllObjectsByType(
    string type, bool includeInactive = false) const -> vector<Object *>;

  // Returns a list of object colliding with the object at a given position
  auto getCollisions(Object *o, Vector where) const -> vector<Object *>;

  // Moves an object to a given position and resolves
  void resolveMovement(Object *o, Vector position);

  // Updates all active objects and frees resources for deleted ones
  void update();

  // Marks an object to be deleted in the next update call
  auto markForDelete(Object *o) -> int;

  template <typename T, typename... Args>
  static auto create(Args &&...args) -> T * {
    static_assert(std::is_base_of<Object, T>::value,
                  "T must inherit from Object");
    auto obj = std::make_unique<T>(std::forward<Args>(args)...);
    T *ptr = obj.get();
    getInstance().sceneGraph.insertObject(std::move(obj));
    return ptr;
  }

  // Draws all the active objects in the view
  void draw();

  // Set the boundary for the world
  void setBoundary(Box b);
  // Get the boundary for the world
  [[nodiscard]] auto getBoundary() const -> Box;

  // Returns the current SceneGraph
  [[nodiscard]] auto getSceneGraph() -> SceneGraph &;
  // Returns the current View
  [[nodiscard]] auto getView() -> View &;
};
}  // namespace lb
