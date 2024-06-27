#pragma once
#include <string>

#include "Event.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/Object.h"

namespace lb {

const std::string COLLISION_EVENT = "lb::collision";

class EventCollision : public Event {
 private:
  // Position where the collision occurs
  const Vector position = Vector();
  // First object in collision
  const Object *firstObject = nullptr;
  // Second object in collision
  const Object *secondObject = nullptr;

 public:
  EventCollision() : Event(COLLISION_EVENT){};
  // Creates a collision event between two objects at a given position
  EventCollision(Object *o1, Object *o2, Vector pos)
    : Event(COLLISION_EVENT),
      position(pos),
      firstObject(o1),
      secondObject(o2){};

  // Returns the first object involved in the collision
  [[nodiscard]] auto getFirstObject() const -> const Object *;

  // Returns the second object involved in the collision
  [[nodiscard]] auto getSecondObject() const -> const Object *;

  // Returns the position where the collision occurs
  [[nodiscard]] auto getPosition() const -> const Vector;
};

}  // namespace lb
