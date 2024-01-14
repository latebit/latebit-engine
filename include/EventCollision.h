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
  auto getObject1() const -> Object *;

  void setObject2(Object *new_o2);
  auto getObject2() const -> Object *;

  void setPos(Vector pos);
  auto getPos() const -> Vector;
};

}  // namespace df
