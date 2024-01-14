#include "EventCollision.h"

namespace df {

EventCollision::EventCollision() {
  setType(COLLISION_EVENT);
  this->p_obj1 = nullptr;
  this->p_obj2 = nullptr;
  this->pos = Vector();
}

EventCollision::EventCollision(Object *p_o1, Object *p_o2, Vector pos) {
  setType(COLLISION_EVENT);
  this->p_obj1 = p_o1;
  this->p_obj2 = p_o2;
  this->pos = pos;
}

void EventCollision::setObject1(Object *p_new_o1) { this->p_obj1 = p_new_o1; }
Object *EventCollision::getObject1() const { return this->p_obj1; }

void EventCollision::setObject2(Object *p_new_o2) { this->p_obj2 = p_new_o2; }
Object *EventCollision::getObject2() const { return this->p_obj2; }

void EventCollision::setPos(Vector pos) { this->pos = pos; }
Vector EventCollision::getPos() const { return this->pos; }

}  // namespace df