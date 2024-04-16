#include "core/GameManager.h"

#include <cstdio>

#include "../lib/test.h"
#include "core/graphics/DisplayManager.h"
#include "core/objects/WorldManager.h"

using namespace lb;

void GameManager_test() {
  printf("lol");
  GM.startUp();
  printf("alol");
  assert("starts up", GM.isStarted());
  printf("blol");

  printf("clol");
  Box wanted(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  printf("lol");
  assertEq("boundary is set up", WM.getBoundary(), wanted);
  printf("lol1");
  assertEq("view is set up", WM.getView(), wanted);
  printf("lol2");

  test("won't run if not started", []() {
    GM.shutDown();
    printf("lol3");
    GM.run();
    printf("lol4");
    // Reaching this function means the test passed
    assert("doesn't run", !GM.isStarted());
    printf("lol5");
  });

  GM.shutDown();
}