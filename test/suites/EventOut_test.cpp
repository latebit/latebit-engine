#include "EventOut.h"

#include "../lib/test.h"

using namespace df;

void EventOut_test() {
  EventOut event_out;
  assert_string("is initialised with type", event_out.getType(), OUT_EVENT);
};