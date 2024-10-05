#pragma once

#include <array>
#include <memory>
#include <vector>

#include "latebit/core/objects/Object.h"

namespace lb {
const int MAX_ALTITUDE = 4;

class SceneGraph {
 private:
  // All objects
  vector<unique_ptr<Object>> all = {};
  
  // Active objects in the scene
  vector<Object*> active = {};

  // Inactive objects in the scene
  vector<Object*> inactive = {};

  // Solid objects in the scene
  vector<Object*> solid = {};

  // Visible objects in the scene
  array<vector<Object*>, MAX_ALTITUDE + 1> visible = {
    vector<Object*>{}, vector<Object*>{}, vector<Object*>{}, vector<Object*>{}, vector<Object*>{}
  };

 public:
  SceneGraph();
  ~SceneGraph() = default;

  // Insert object in the scene
  auto insertObject(unique_ptr<Object> o) -> int;

  // Remove object from the scene
  auto removeObject(Object *o) -> int;

  // Returns active objects
  [[nodiscard]] auto getActiveObjects() const -> vector<Object*>;

  // Returns inactive objects
  [[nodiscard]] auto getInactiveObjects() const -> vector<Object*>;

  // Returns all active solid objects
  [[nodiscard]] auto getSolidObjects() const -> vector<Object*>;

  // Returns all active visible objects on a given rendering layer
  [[nodiscard]] auto getVisibleObjects(int altitude) const -> vector<Object*>;

  [[nodiscard]] auto getAllObjects() const -> const vector<unique_ptr<Object>>&;

  // Update solidness for a given object
  auto setSolidness(Object *o, Solidness::Solidness solidness) -> int;

  // Update rendering layer for a given object
  auto setAltitude(Object *o, int altitude) -> int;

  // Update visibility for a given object
  auto setVisible(Object *o, bool isVisible) -> int;

  // Marks an object as active
  auto setActive(Object *o, bool isActive) -> int;

  void clear();
};
}  // namespace lb
