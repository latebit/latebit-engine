#include "GameManager.h"

#include "../lib/test.h"
#include "DisplayManager.h"
#include "LogManager.h"
#include "WorldManager.h"

using namespace df;

void GameManager_test() {
  GM.startUp();
  assert("starts up", GM.isStarted());

  Box wanted(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  assert_box("boundary is set up", WM.getBoundary(), wanted);
  assert_box("view is set up", WM.getView(), wanted);

  GM.shutDown();
  // Restore log manager to continue testing
  LM.startUp();
}