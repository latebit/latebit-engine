#include "latebit/core/GameManager.h"

#include "../../test/lib/test.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/WorldManager.h"

using namespace lb;

auto main() -> int {
  test("when starting up", []() {
    GM.startUp();
    assert("starts up", GM.isStarted());

    Box wanted(Vector(0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);
    assertEq("boundary is set up", WM.getBoundary(), wanted);
    assertEq("view is set up", WM.getView().getView(), wanted);
    assertEq("is not over", GM.getGameOver(), false);
    assertEq("is not paused", GM.isPaused(), false);

    GM.shutDown();
  });

  test("when not started not started", []() {
    GM.startUp();
    GM.shutDown();
    GM.run();
    // Reaching this function means the test passed
    assertEq("doesn't run", GM.isStarted(), false);
  });

  test("can end the game", []() {
    GM.startUp();
    GM.setGameOver();
    assertEq("game is over", GM.getGameOver(), true);
    GM.shutDown();
  });

  test("can pause the game", []() {
    GM.startUp();

    GM.pause();
    assertEq("game is paused", GM.isPaused(), true);

    GM.resume();
    assertEq("game is not paused", GM.isPaused(), false);

    GM.shutDown();
  });

  return report();
}