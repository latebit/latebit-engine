#include "latebit/core/events/Event.h"

#include "../../../test/lib/test.h"

using namespace lb;

auto main() -> int {
  Event subject;

  subject.setType("type");
  assertEq("updates type", subject.getType(), "type");
  return report();
}