#include "../include/Event.h"

#include "test.h"

int Event_test() {
  auto *subject = new df::Event;
  int result = 0;

  subject->setType("type");
  result += assert_string("updates type", subject->getType(), "type");

  return result;
}