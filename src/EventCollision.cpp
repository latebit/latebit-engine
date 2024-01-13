#include "EventCollision.h"

namespace df {

EventCollision::EventCollision() {
  setType(COLLISION_EVENT);
  m_p_obj1 = nullptr;
  m_p_obj2 = nullptr;
  m_pos = Vector();
}

EventCollision::EventCollision(Object *p_o1, Object *p_o2, Vector pos) {
  setType(COLLISION_EVENT);
  m_p_obj1 = p_o1;
  m_p_obj2 = p_o2;
  m_pos = pos;
}

void EventCollision::setObject1(Object *p_new_o1) { m_p_obj1 = p_new_o1; }
Object *EventCollision::getObject1() const { return m_p_obj1; }

void EventCollision::setObject2(Object *p_new_o2) { m_p_obj2 = p_new_o2; }
Object *EventCollision::getObject2() const { return m_p_obj2; }

void EventCollision::setPos(Vector pos) { m_pos = pos; }
Vector EventCollision::getPos() const { return m_pos; }

}  // namespace df