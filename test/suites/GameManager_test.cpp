#include "GameManager.h"

#include "../lib/test.h"
#include "DisplayManager.h"
#include "Logger.h"
#include "WorldManager.h"

using namespace lb;

void GameManager_test() {
  GM.startUp();
  Debug("started");
  assert("starts up", GM.isStarted());

  Box wanted(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  assert_box("boundary is set up", WM.getBoundary(), wanted);
  assert_box("view is set up", WM.getView(), wanted);

  GM.shutDown();
}