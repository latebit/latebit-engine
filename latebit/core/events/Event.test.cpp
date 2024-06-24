#include "latebit/core/events/Event.h"

#include "../../../test/lib/test.h"

using namespace lb;

auto main() -> int {
  Event subject("type");
  assertEq("updates type", subject.getType(), "type");

  Event undef;
  assertEq("updates type", undef.getType(), UNDEFINED_EVENT);

  return report();
}