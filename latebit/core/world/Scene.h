#include "latebit/core/world/SceneGraph.h"

#include <memory>

namespace lb {
class Scene {
 private:
  // All objects. This vector owns all the objects in the graph and its
  // responsible for their memory management
  vector<unique_ptr<Object>> objects = {};
  SceneGraph& graph;

  void activate();
  void deactivate();

 public:
  Scene(SceneGraph& graph);
  virtual void onActivated() {};
  virtual void onDeactivated() {};

  // Add object to the scene
  void addObject(unique_ptr<Object> o);
  // Remove object from the scene
  void removeObject(Object* o);
};
}  // namespace lb