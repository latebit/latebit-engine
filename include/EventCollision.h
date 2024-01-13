#pragma once
#include <string>

#include "Event.h"
#include "Object.h"
#include "Vector.h"

namespace df {

const std::string COLLISION_EVENT = "df::collision";

class EventCollision : public Event {
 private:
  Vector m_pos;
  Object *m_p_obj1;
  Object *m_p_obj2;

 public:
  EventCollision();
  EventCollision(Object *p_o1, Object *p_o2, Vector pos);

  void setObject1(Object *p_new_o1);
  Object *getObject1() const;

  void setObject2(Object *p_new_o2);
  Object *getObject2() const;

  void setPos(Vector pos);
  Vector getPos() const;
};

}  // namespace df
