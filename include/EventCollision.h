#pragma once
#include <string>

#include "Event.h"
#include "Object.h"
#include "Vector.h"

namespace lb {

const std::string COLLISION_EVENT = "lb::collision";

class EventCollision : public Event {
 private:
  // Position where the collision occurs
  Vector position = Vector();
  // First object in collision
  Object *firstObject = nullptr;
  // Second object in collision
  Object *secondObject = nullptr;

 public:
  EventCollision();
  // Creates a collision event between two objects at a given position
  EventCollision(Object *o1, Object *o2, Vector pos);

  // Updates the first object involved in the collision
  void setFirstObject(Object *o);
  // Returns the first object involved in the collision
  [[nodiscard]] auto getFirstObject() const -> Object *;

  // Updates the second object involved in the collision
  void setSecondObject(Object *o2);
  // Returns the second object involved in the collision
  [[nodiscard]] auto getSecondObject() const -> Object *;

  // Updates the position where the collision occurs
  void setPosition(Vector pos);
  // Returns the position where the collision occurs
  [[nodiscard]] auto getPos() const -> Vector;
};

}  // namespace lb
