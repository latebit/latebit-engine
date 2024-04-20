#include "latebit/core/events/EventOut.h"

#include "../../../test/lib/test.h"

using namespace lb;

auto main() -> int {
  EventOut evt;
  assertEq("is initialised with type", evt.getType(), OUT_EVENT);
  return report();
};