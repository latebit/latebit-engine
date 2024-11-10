#pragma once
#include <string>

#include "Event.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"

namespace lb {

const std::string OVERLAP_EVENT = "lb::overlap";

class EventOverlap : public Event {
 private:
  // Position where the collision occurs
  const Vector position = Vector();
  // First object in collision
  const Object *one = nullptr;
  // Second object in collision
  const Object *two = nullptr;

 public:
  // Creates an overlap event between two objects at a given position
  // Even though the objects are called one and two, the order is arbitrary.
  EventOverlap(Object *o1, Object *o2, Vector p)
    : Event(OVERLAP_EVENT), position(p), one(o1), two(o2){};

  // Returns the first object involved in the collision
  [[nodiscard]] auto getFirstObject() const -> const Object * {
    return this->one;
  };

  // Returns the second object involved in the collision
  [[nodiscard]] auto getSecondObject() const -> const Object * {
    return this->two;
  };

  // Returns the position where the collision occurs
  [[nodiscard]] auto getPosition() const -> const Vector {
    return this->position;
  };
};

}  // namespace lb
