#include "core/events/EventOut.h"

#include "../lib/test.h"

using namespace lb;

void EventOut_test() {
  EventOut event_out;
  assertEq("is initialised with type", event_out.getType(), OUT_EVENT);
};