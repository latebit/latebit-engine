#include "EventCollision.h"

namespace lb {

EventCollision::EventCollision() { setType(COLLISION_EVENT); }

EventCollision::EventCollision(Object *o1, Object *o2, Vector pos) {
  setType(COLLISION_EVENT);
  this->firstObject = o1;
  this->secondObject = o2;
  this->position = pos;
}

void EventCollision::setFirstObject(Object *o) { this->firstObject = o; }
auto EventCollision::getFirstObject() const -> Object * {
  return this->firstObject;
}

void EventCollision::setSecondObject(Object *o) { this->secondObject = o; }
auto EventCollision::getSecondObject() const -> Object * {
  return this->secondObject;
}

void EventCollision::setPosition(Vector position) { this->position = position; }
auto EventCollision::getPos() const -> Vector { return this->position; }

}  // namespace lb