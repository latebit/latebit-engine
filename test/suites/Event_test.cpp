#include "Event.h"

#include "../lib/test.h"

using namespace lb;

void Event_test() {
  Event subject;

  subject.setType("type");
  assert_string("updates type", subject.getType(), "type");
}