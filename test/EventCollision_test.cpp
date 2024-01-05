#include "../includes/EventCollision.h"

#include "test.h"

int EventCollision_test() {
  int result = 0;

  // Test default constructor
  df::EventCollision subject;
  result += assert("sets default object1", subject.getObject1() == nullptr);
  result += assert("sets default object2", subject.getObject2() == nullptr);
  result +=
      assert_vector("sets default position", subject.getPos(), df::Vector());

  // Test constructor with parameters
  df::Object obj1, obj2;
  df::Vector pos(1, 1);
  subject = df::EventCollision(&obj1, &obj2, pos);
  result += assert("sets correct object1", subject.getObject1() == &obj1);
  result += assert("sets correct object2", subject.getObject2() == &obj2);
  result += assert_vector("sets correct position", subject.getPos(), pos);

  // Test setObject1()
  df::Object obj3;
  subject.setObject1(&obj3);
  result += assert("sets correct object1", subject.getObject1() == &obj3);

  // Test setObject2()
  df::Object obj4;
  subject.setObject2(&obj4);
  result += assert("sets correct object2", subject.getObject2() == &obj4);

  // Test setPos()
  df::Vector pos2(2, 2);
  subject.setPos(pos2);
  result += assert_vector("sets correct position", subject.getPos(), pos2);

  return result;
}