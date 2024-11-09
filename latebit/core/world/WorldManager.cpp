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

void WorldManager::resolveMovement(Object *o, Vector position) {
  // Static objects don't move
  if (o->getBodyType() == BodyType::STATIC) return;

  // Kinematic objects don't participate in collisions nor overlaps and can
  // always move
  if (o->getBodyType() == BodyType::KINEMATIC) {
    return moveAndCheckBounds(o, position);
  }

  vector<Object *> collisions = getCollisions(o, position);

  // In absence of collisions, just move
  if (collisions.empty()) {
    return moveAndCheckBounds(o, position);
  }

  for (auto o2 : collisions) {
    auto event = EventCollision(o, o2, position);
    o->eventHandler(&event);
    o2->eventHandler(&event);

    if (o->getBodyType() == BodyType::DYNAMIC) {
      if (o2->getBodyType() == BodyType::DYNAMIC) {
        auto bounciness = min(o->getBounciness(), o2->getBounciness());

        auto v1 = o->getVelocity();
        auto v2 = o2->getVelocity();
        auto m1 = o->getMass();
        auto m2 = o2->getMass();

        auto direction = (o2->getPosition() - position).normalize();
        auto velocityAlongNormal = (v2 - v1).dot(direction);

        if (velocityAlongNormal > 0) continue;

        auto inverseM1 = 1 / m1;
        auto inverseM2 = 1 / m2;
        auto impulse =
          (velocityAlongNormal * -(1 + bounciness)) / (inverseM1 + inverseM2);
        auto d1 = direction * impulse * inverseM1;
        auto d2 = direction * impulse * inverseM2;

        o->setVelocity(v1 - d1);
        o2->setVelocity(v2 + d2);
        return;
      }

      if (o2->getBodyType() == BodyType::STATIC) {
        auto bounciness = min(o->getBounciness(), o2->getBounciness());

        auto v1 = o->getVelocity();
        auto m1 = o->getMass();

        // Calculate the overlap along each axis
        auto box1 = o->getBox();
        auto box2 = o2->getBox();
        auto center1 =
          o->getPosition() + Vector(box1.getWidth() / 2, box1.getHeight() / 2);
        auto center2 =
          o2->getPosition() + Vector(box2.getWidth() / 2, box2.getHeight() / 2);

        float overlapX = (box1.getWidth() / 2 + box2.getWidth() / 2) -
                         abs(center1.getX() - center2.getX());
        float overlapY = (box1.getHeight() / 2 + box2.getHeight() / 2) -
                         abs(center1.getY() - center2.getY());

        Vector normal =
          overlapX < overlapY
            ? Vector((center1.getX() > center2.getX()) ? 1 : -1, 0)
            : Vector(0, (center1.getY() > center2.getY()) ? 1 : -1);

        auto velocityAlongNormal = v1.dot(normal);

        // Skip if the object is moving away
        if (velocityAlongNormal > 0) continue;

        auto normalVelocity = normal * velocityAlongNormal;
        auto tangentVelocity = v1 - normalVelocity;

        auto inverseM1 = 1 / m1;
        auto impulse = (velocityAlongNormal * -(1 + bounciness)) / (inverseM1);
        auto deltaV1 = normal * impulse * inverseM1;

        o->setVelocity(tangentVelocity - (normalVelocity + deltaV1));
        return moveAndCheckBounds(o, o->getPosition() + o->getVelocity());
      }
    }
  }

  moveAndCheckBounds(o, position);
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

  auto active = this->sceneGraph.getActiveObjects();
  for (auto &object : active) {
    auto oldPosition = object->getPosition();
    auto newVelocity = object->getVelocity() + object->getAcceleration();
    auto newPosition = object->getPosition() + newVelocity;

    object->setVelocity(newVelocity);
    if (oldPosition != newPosition) {
      this->resolveMovement(object, newPosition);
    }
  }
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
