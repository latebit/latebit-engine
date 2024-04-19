#include "core/GameManager.h"

#include <cstdio>

#include "../../test/lib/test.h"
#include "core/graphics/DisplayManager.h"
#include "core/objects/WorldManager.h"

using namespace lb;

auto main() -> int {
  GM.startUp();
  assert("starts up", GM.isStarted());

  Box wanted(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  assertEq("boundary is set up", WM.getBoundary(), wanted);
  assertEq("view is set up", WM.getView(), wanted);

  test("won't run if not started", []() {
    GM.shutDown();
    GM.run();
    // Reaching this function means the test passed
    assert("doesn't run", !GM.isStarted());
  });

  GM.shutDown();

  return report();
}