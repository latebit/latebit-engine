#include "WorldManager.h"

#include <memory>
#include <string>
#include <vector>

#include "latebit/core/utils/utils.h"
#include "latebit/core/world/Camera.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/ObjectUtils.h"
#include "latebit/core/world/Physics.h"
#include "latebit/utils/Logger.h"

#define WM lb::WorldManager::getInstance()

namespace lb {

WorldManager::WorldManager() : Manager("WorldManager") {
  Log.debug("WorldManager::WorldManager(): Created WorldManager");
}

auto WorldManager::getInstance() -> WorldManager & {
  static WorldManager instance;
  return instance;
}

auto WorldManager::startUp() -> int {
  this->scenes.clear();
  this->sceneGraph.clear();
  this->camera = Camera(this);
  Log.info("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  this->scenes.clear();
  this->sceneGraph.clear();
  Manager::shutDown();
  Log.info("WorldManager::shutDown(): Shut down successfully");
}

auto WorldManager::isValid([[maybe_unused]] string eventType) const -> bool {
  // World Manager needs to accept all the events because it is the Manager
  // responsible for registering custom events
  return true;
}

auto WorldManager::getAllObjects(bool includeInactive) const
  -> vector<Object *> {
  if (includeInactive) {
    auto active = this->sceneGraph.getActiveObjects();
    auto inactive = this->sceneGraph.getInactiveObjects();
    active.insert(active.end(), inactive.begin(), inactive.end());
    return active;
  }

  return this->sceneGraph.getActiveObjects();
}

auto WorldManager::getAllObjectsByType(std::string type,
                                       bool includeInactive) const
  -> vector<Object *> {
  vector<Object *> result = {};
  auto all = this->getAllObjects(includeInactive);
  result.reserve(all.size());

  for (auto object : all) {
    if (object->getType() == type) {
      result.push_back(object);
    }
  }

  return result;
}

void WorldManager::update() {
  if (!isStarted()) return;

  // Delete objects marked for deletion
  for (auto &object : deletions) {
    this->sceneGraph.removeObject(object);
  }
  deletions.clear();

  this->physics.update();
  // Note: camera update must be called after physics update, else we'd be
  // following objects one tick behind
  this->camera.update();
}

auto WorldManager::markForDelete(Object *o) -> int {
  o->unsubscribeAll();
  o->teardown();
  insert(deletions, o);
  return 0;
}

void WorldManager::draw() {
  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    auto visible = this->sceneGraph.getVisibleObjects(i);
    for (auto &o : visible) {
      if (o != nullptr &&
          intersects(o->getWorldBox(), this->camera.getView())) {
        o->draw();
      };
    }
  }
}

void WorldManager::setBoundary(Box b) { this->boundary = b; }
auto WorldManager::getBoundary() const -> Box { return this->boundary; }

auto WorldManager::getSceneGraph() -> SceneGraph & { return this->sceneGraph; }
auto WorldManager::getCamera() -> Camera & { return this->camera; }
auto WorldManager::getPhysics() -> Physics & { return this->physics; }

auto WorldManager::activateScene(const string label) -> int {
  for (auto &scene : this->scenes) {
    if (scene->label == label) {
      scene->activate();
      return 0;
    }
  }
  Log.error(
    "WorldManager::activateScene(): Could not activate scene %s. Scene not "
    "found",
    label.c_str());
  return -1;
}

auto WorldManager::deactivateScene(const string label) -> int {
  for (auto &scene : this->scenes) {
    if (scene->label == label) {
      scene->deactivate();
      return 0;
    }
  }
  Log.error(
    "WorldManager::deactivateScene(): Could not deactivate scene %s. Scene not "
    "found",
    label.c_str());
  return -1;
}

auto WorldManager::switchToScene(const string label) -> int {
  for (auto &scene : this->scenes) {
    if (scene->isActive()) scene->deactivate();
    if (scene->label == label) scene->activate();
  }
  return 0;
}

auto WorldManager::getScenes() const -> const vector<unique_ptr<Scene>> & {
  return this->scenes;
}

}  // namespace lb
