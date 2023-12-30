#include "../src/Object.h"

#include "test.h"

int Object_test() {
  df::Object *subject;
  int result = 0;

  subject = new df::Object;
  result += assert_int("sets an id", subject->getId(), 0);
  subject = new df::Object;
  result += assert_int("increments id", subject->getId(), 1);
  subject->setId(10);
  result += assert_int("updates id", subject->getId(), 10);

  subject->setType("type");
  result += assert_string("updates type", subject->getType(), "type");

  auto position = new df::Vector(1, 2);
  subject->setPosition(*position);
  auto got = subject->getPosition();
  result += assert_vector("updates position", &got, position);

  return result;
}