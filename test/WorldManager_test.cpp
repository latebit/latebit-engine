#include "../src/WorldManager.h"

#include "test.h"

int WorldManager_test() {
  int result = 0;

  auto enemy1 = new df::Object;
  auto enemy2 = new df::Object;
  auto enemy3 = new df::Object;
  auto enemy4 = new df::Object;

  enemy3->setType("type");
  enemy4->setType("type");

  WM.startUp();

  WM.insertObject(enemy1);
  WM.insertObject(enemy2);
  WM.insertObject(enemy3);
  WM.insertObject(enemy4);
  WM.update();

  result += assert_int("has all the objects", WM.getAllObjects().getCount(), 4);
  result += assert_int("filters objects by type",
                       WM.objectsOfType("type").getCount(), 2);

  WM.markForDelete(enemy1);
  WM.update();
  result += assert_int("has one less object", WM.getAllObjects().getCount(), 3);

  WM.removeObject(enemy2);
  WM.update();
  result += assert_int("removes an object", WM.getAllObjects().getCount(), 2);

  WM.shutDown();
  result += assert_int("removes everything", WM.getAllObjects().getCount(), 0);

  return result;
}