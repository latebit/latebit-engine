#pragma once

#include <array>

#include "Object.h"
#include "ObjectList.h"

namespace lb {
const int MAX_ALTITUDE = 4;

class SceneGraph {
 private:
  // Active objects in the scene
  ObjectList active = ObjectList();

  // Inactive objects in the scene
  ObjectList inactive = ObjectList();

  // Solid objects in the scene
  ObjectList solid = ObjectList();

  // Visible objects in the scene
  array<ObjectList, MAX_ALTITUDE + 1> visible = {};

 public:
  SceneGraph();
  ~SceneGraph();

  // Insert object in the scene
  auto insertObject(Object *o) -> int;

  // Remove object from the scene
  auto removeObject(Object *o) -> int;

  // Returns active objects
  [[nodiscard]] auto getActiveObjects() const -> ObjectList;

  // Returns inactive objects
  [[nodiscard]] auto getInactiveObjects() const -> ObjectList;

  // Returns all active solid objects
  [[nodiscard]] auto getSolidObjects() const -> ObjectList;

  // Returns all active visible objects on a given rendering layer
  [[nodiscard]] auto getVisibleObjects(int altitude) const -> ObjectList;

  // Update solidness for a given object
  auto setSolidness(Object *o, Solidness solidness) -> int;

  // Update rendering layer for a given object
  auto setAltitude(Object *o, int altitude) -> int;

  // Update visibility for a given object
  auto setVisible(Object *o, bool isVisible) -> int;

  // Marks an object as active
  auto setActive(Object *o, bool isActive) -> int;
};
}  // namespace lb
