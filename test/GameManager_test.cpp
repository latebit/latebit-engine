#include "../include/GameManager.h"

#include "../include/DisplayManager.h"
#include "../include/LogManager.h"
#include "../include/WorldManager.h"
#include "test.h"

auto GameManager_test() -> int {
  int result = 0;

  GM.startUp();
  result += assert("starts up", GM.isStarted());

  Box wanted(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  result += assert_box("boundary is set up", WM.getBoundary(), wanted);
  result += assert_box("view is set up", WM.getView(), wanted);

  GM.shutDown();
  LM.startUp();

  return result;
}