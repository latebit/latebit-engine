#include "WorldManager.h"

#include <memory>
#include <string>
#include <vector>

#include "latebit/core/events/EventCollision.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/utils/utils.h"
#include "latebit/core/world/Camera.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/ObjectUtils.h"
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

auto WorldManager::getAllObjectsByType(
  std::string type, bool includeInactive) const -> vector<Object *> {
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

auto WorldManager::getCollisions(Object *o,
                                 Vector where) const -> vector<Object *> {
  vector<Object *> collisions = {};
  auto solid = this->sceneGraph.getSolidObjects();
  auto box = o->getWorldBox(where);
  collisions.reserve(solid.size());

  for (auto current : solid) {
    if (current == o) continue;
    auto currentBox = current->getWorldBox();

    if (intersects(box, currentBox)) {
      collisions.push_back(current);
    }
  }

  return collisions;
}

void WorldManager::updatePhysics() {
  auto active = this->sceneGraph.getActiveObjects();
  for (auto &object : active) {
    auto newVelocity = object->getVelocity() + object->getAcceleration();
    object->setVelocity(newVelocity);

    auto oldPosition = object->getPosition();
    auto newPosition = object->getPosition() + newVelocity;

    // No need to resolve movement is there is no movement.
    if (oldPosition == newPosition) continue;

    // Non-solid objects don't collide and can always move
    if (!object->isSolid()) {
      moveAndCheckBounds(object, newPosition);
      continue;
    }

    const auto collisions = getCollisions(object, newPosition);

    // In absence of collisions, just move
    if (collisions.empty()) {
      moveAndCheckBounds(object, newPosition);
      continue;
    }

    // Each collision can update these vectors, so we need them outside the loop
    Vector p1 = newPosition;
    Vector v1 = object->getVelocity();

    for (auto &collider : collisions) {
      auto event = EventCollision(object, collider, newPosition);
      object->eventHandler(&event);
      collider->eventHandler(&event);

      const bool needsCollisionResolution =
        object->getSolidness() == Solidness::HARD &&
        collider->getSolidness() == Solidness::HARD;

      if (!needsCollisionResolution) continue;

      // Collisions are resolved by applying the minimum displacement necessary
      // to push the objects apart. This is done by calculating the overlap
      // between the two objects and moving the object in the direction of the
      // smaller overlap.

      const Vector p2 = collider->getPosition();
      const Box b1 = object->getWorldBox(p1);
      const Box b2 = collider->getWorldBox();
      const Vector c1 = b1.getCenter();
      const Vector c2 = b2.getCenter();

      // The minimum distance between the two objects for them to not overlap
      const float nonOverlappingDistanceX = (b1.getWidth() + b2.getWidth()) / 2;
      const float nonOverlappingDistanceY =
        (b1.getHeight() + b2.getHeight()) / 2;

      const float distanceX = abs(c1.getX() - c2.getX());
      const float distanceY = abs(c1.getY() - c2.getY());

      const Vector overlap = {nonOverlappingDistanceX - distanceX,
                              nonOverlappingDistanceY - distanceY};

      if (overlap.getX() < overlap.getY()) {
        if (c1.getX() < c2.getX()) {
          p1.setX(p2.getX() - b1.getWidth());
          v1.setX(min(0.0f, v1.getX()));
        } else {
          p1.setX(p2.getX() + b2.getWidth());
          v1.setX(max(0.0f, v1.getX()));
        }
      } else {
        if (c1.getY() < c2.getY()) {
          p1.setY(p2.getY() - b1.getHeight());
          v1.setY(min(0.0f, v1.getY()));
        } else {
          p1.setY(p2.getY() + b2.getHeight());
          v1.setY(max(0.0f, v1.getY()));
        }
      }
    }

    moveAndCheckBounds(object, p1);
    object->setVelocity(v1);
  }
}

void WorldManager::moveAndCheckBounds(Object *o, Vector position) {
  auto initial = o->getWorldBox();
  o->setPosition(position);
  auto final = o->getWorldBox();
  auto boundary = WM.getBoundary();

  if (intersects(initial, boundary) && !intersects(final, boundary)) {
    auto event = EventOut();
    o->eventHandler(&event);
  }

  if (this->camera.getViewFollowing() == o) {
    auto viewDeadZone = this->camera.getViewDeadZone();

    // Move the view if the object is outside of the dead zone
    if (!contains(viewDeadZone, final)) {
      this->camera.setViewPosition(position);
    }
  }
}

void WorldManager::update() {
  if (!isStarted()) return;

  // Delete objects marked for deletion
  for (auto &object : deletions) {
    this->sceneGraph.removeObject(object);
  }
  deletions.clear();

  this->updatePhysics();
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
