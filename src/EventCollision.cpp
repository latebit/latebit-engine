#include "EventCollision.h"

namespace df {

EventCollision::EventCollision() {
  setType(COLLISION_EVENT);
  this->obj1 = nullptr;
  this->obj2 = nullptr;
  this->pos = Vector();
}

EventCollision::EventCollision(Object *o1, Object *o2, Vector pos) {
  setType(COLLISION_EVENT);
  this->obj1 = o1;
  this->obj2 = o2;
  this->pos = pos;
}

void EventCollision::setObject1(Object *new_o1) { this->obj1 = new_o1; }
Object *EventCollision::getObject1() const { return this->obj1; }

void EventCollision::setObject2(Object *new_o2) { this->obj2 = new_o2; }
Object *EventCollision::getObject2() const { return this->obj2; }

void EventCollision::setPos(Vector pos) { this->pos = pos; }
Vector EventCollision::getPos() const { return this->pos; }

}  // namespace df