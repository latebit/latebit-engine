#pragma once
#include <string>

#include "Event.h"
#include "Object.h"
#include "Vector.h"

namespace df {

const std::string COLLISION_EVENT = "df::collision";

class EventCollision : public Event {
 private:
  Vector pos;
  Object *obj1;
  Object *obj2;

 public:
  EventCollision();
  EventCollision(Object *o1, Object *o2, Vector pos);

  void setObject1(Object *new_o1);
  Object *getObject1() const;

  void setObject2(Object *new_o2);
  Object *getObject2() const;

  void setPos(Vector pos);
  Vector getPos() const;
};

}  // namespace df
