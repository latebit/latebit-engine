#include "Scene.h"

#include <algorithm>

namespace lb {
Scene::Scene(SceneGraph& graph) : graph(graph) {}

void Scene::activate() {
  for (auto& o : this->objects) {
    this->graph.insertObject(o.get());
  }
  onActivated();
}

void Scene::deactivate() {
  this->graph.clear();
  onDeactivated();
}

void Scene::addObject(unique_ptr<Object> o) {
  this->objects.push_back(std::move(o));
  this->graph.insertObject(o.get());
}

void Scene::removeObject(Object* o) {
  this->objects.erase(
    std::remove_if(this->objects.begin(), this->objects.end(),
                   [o](const unique_ptr<Object>& p) { return p.get() == o; }),
    this->objects.end());
  this->graph.removeObject(o);
}
}  // namespace lb