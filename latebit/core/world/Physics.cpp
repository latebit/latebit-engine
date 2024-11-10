#include "Physics.h"
#include "latebit/core/events/EventCollision.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/events/EventOverlap.h"
#include "latebit/core/utils/utils.h"
#include "latebit/core/world/WorldManager.h"

namespace lb {
void Physics::update() {
  auto active = this->sceneGraph->getActiveObjects();
  for (auto &object : active) {
    auto newVelocity = object->getVelocity() + object->getAcceleration();
    object->setVelocity(newVelocity);

    auto oldPosition = object->getPosition();
    auto newPosition = object->getPosition() + newVelocity;

    // No need to resolve movement is there is no movement.
    if (oldPosition == newPosition) continue;

    // Non-solid objects don't collide and can always move
    if (!object->isSolid()) {
      moveTo(object, newPosition);
      continue;
    }

    const auto overlaps = this->getOverlapsAt(object, newPosition);

    // In absence of overlaps, just move
    if (overlaps.empty()) {
      moveTo(object, newPosition);
      continue;
    }

    // Collisions can update these vectors, so we need them mutable and outside the loop
    Vector p1 = newPosition;
    Vector v1 = object->getVelocity();

    for (auto &overlapper : overlaps) {
      auto overlapEvent = EventOverlap(object, overlapper, newPosition);
      object->eventHandler(&overlapEvent);
      overlapper->eventHandler(&overlapEvent);

      const bool needsCollisionResolution =
        object->getSolidness() == Solidness::HARD &&
        overlapper->getSolidness() == Solidness::HARD;

      if (!needsCollisionResolution) continue;

      auto collisionEvent = EventCollision(object, overlapper, newPosition);
      object->eventHandler(&collisionEvent);
      overlapper->eventHandler(&collisionEvent);

      // Collisions are resolved by applying the minimum displacement necessary
      // to push the objects apart. This is done by calculating the overlap
      // between the two objects and moving the object in the direction of the
      // smaller overlap.

      const Vector p2 = overlapper->getPosition();
      const Box b1 = object->getWorldBox(p1);
      const Box b2 = overlapper->getWorldBox();
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

    moveTo(object, p1);
    object->setVelocity(v1);
  }
}

auto Physics::getOverlapsAt(Object *o,
                            Vector where) const -> vector<Object *> {
  vector<Object *> collisions = {};
  auto solid = this->sceneGraph->getSolidObjects();
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

auto Physics::moveTo(Object *o, Vector position) -> void {
  auto initial = o->getWorldBox();
  o->setPosition(position);
  auto final = o->getWorldBox();
  auto boundary = this->world->getBoundary();

  if (intersects(initial, boundary) && !intersects(final, boundary)) {
    auto event = EventOut();
    o->eventHandler(&event);
  }
}

}  // namespace lb