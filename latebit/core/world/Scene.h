#pragma once

#include <memory>
#include <string>

#include "latebit/core/world/SceneGraph.h"

namespace lb {
class WorldManager;
class Scene {
 private:
  // All objects. This vector owns all the objects in the graph and its
  // responsible for their memory management
  vector<unique_ptr<Object>> objects = {};
  bool active = false;
  SceneGraph* graph = nullptr;
  string label = "";
  void setSceneGraph(SceneGraph& graph);
  void setLabel(const string label);
  friend class WorldManager;

 public:
  Scene() = default;
  virtual ~Scene() = default;
  virtual void onActivated() {};
  virtual void onDeactivated() {};

  // Add object to the scene
  void addObject(unique_ptr<Object> o);
  // Remove object from the scene
  void removeObject(Object* o);

  // Runs the scene activation sequence (i.e., add objects to the graph)
  void activate();
  // Runs the scene deactivation sequence
  void deactivate();

  // Returns the objects in the scene
  [[nodiscard]] auto getObjects() const -> const vector<unique_ptr<Object>>&;
  // Returns the scene label
  [[nodiscard]] auto getLabel() const -> const string&;
  // Returns true if the scene is active
  [[nodiscard]] auto isActive() const -> bool;
};
}  // namespace lb