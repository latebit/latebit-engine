#pragma once

#include "latebit/core/world/SceneGraph.h"

#include <memory>
#include <string>

namespace lb {
class WorldManager;
class Scene {
  protected:
  // All objects. This vector owns all the objects in the graph and its
  // responsible for their memory management
  vector<unique_ptr<Object>> objects = {};
  bool active = false;
 
 private:
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
};
}  // namespace lb