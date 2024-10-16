#include "Scene.h"

#include <algorithm>

#include "latebit/core/objects/Object.h"

namespace lb {
void Scene::activate() {
  active = true;
  for (auto& o : this->objects) {
    this->graph->insertObject(o.get());
  }
  onActivated();
}

void Scene::deactivate() {
  active = false;
  for (auto& o : this->objects) {
    this->graph->removeObject(o.get());
  }
  onDeactivated();
}

void Scene::addObject(unique_ptr<Object> o) {
  auto ptr = o.get();
  this->objects.push_back(std::move(o));
  if (active) {
    this->graph->insertObject(ptr);
  }
}

void Scene::removeObject(Object* o) {
  this->objects.erase(
    std::remove_if(this->objects.begin(), this->objects.end(),
                   [o](const unique_ptr<Object>& p) { return p.get() == o; }),
    this->objects.end());
  if (active) {
    this->graph->removeObject(o);
  }
}

void Scene::setSceneGraph(SceneGraph& graph) { this->graph = &graph; }
void Scene::setLabel(const string label) { this->label = label; }
auto Scene::isActive() const -> bool { return active; }
auto Scene::getObjects() const -> const vector<unique_ptr<Object>>& { return objects; }
auto Scene::getLabel() const -> const string& { return label; }

}  // namespace lb