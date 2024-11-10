#include "WorldManager.h"

#include <array>
#include <memory>
#include <vector>

#include "../../../test/lib/test.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"

using namespace std;
using namespace lb;

void draw() {
  WM.startUp();
  static array<int, 5> drawCount = {0, 0, 0, 0, 0};

  class TestObject : public Object {
   public:
    TestObject(int altitude, Vector position = Vector()) {
      setAltitude(altitude);
      setPosition(position);
    }
    auto draw() -> int override {
      // Count the number of drawings per altitude
      drawCount[getAltitude()]++;
      return Object::draw();
    }
  };

  // Initialize objects and results array
  auto scene = WM.createScene<Scene>("main");
  for (int i = 0; i < 5; i++) {
    drawCount[i] = 0;
    scene->createObject<TestObject>(i);
  };
  scene->activate();

  WM.draw();
  for (int i : drawCount) {
    assertEq("draws all objects", i, 1);
  }

  WM.shutDown();
  WM.startUp();

  scene = WM.createScene<Scene>("main");
  scene->createObject<TestObject>(0, Vector(-2, -2));
  WM.draw();
  assertEq("does not draw out of bounds", drawCount[0], 1);

  WM.shutDown();
}

void objectManagement() {
  WM.startUp();
  array<Object *, 5> objects;

  auto scene = WM.createScene<Scene>("main");
  for (int i = 0; i < 5; i++) objects[i] = scene->createObject<Object>();
  scene->activate();

  objects[0]->setType("lol");
  objects[1]->setType("asd");
  objects[2]->setType("type");
  objects[3]->setType("type");
  objects[4]->setType("type");
  objects[4]->setActive(false);

  WM.update();

  auto activeObjects = WM.getAllObjects();
  assertEq("has active objects", activeObjects.size(), 4);
  auto allObjects = WM.getAllObjects(true);
  assertEq("has all the objects", allObjects.size(), 5);
  auto activeTypeObjects = WM.getAllObjectsByType("type");
  assertEq("filters active objects by type", activeTypeObjects.size(), 2);
  auto allTypeObjects = WM.getAllObjectsByType("type", true);
  assertEq("filters all objects by type", allTypeObjects.size(), 3);

  WM.markForDelete(objects[0]);
  WM.update();
  activeObjects = WM.getAllObjects();
  assertEq("has one less object", activeObjects.size(), 3);

  WM.shutDown();
  activeObjects = WM.getAllObjects();
  assertEq("removes everything", activeObjects.size(), 0);

  WM.startUp();
  scene = WM.createScene<Scene>("main");
  auto obj1 = scene->createObject<Object>();

  assert("object is in the scene", scene->getObjects()[0].get() == obj1);
  assertEq("object is not in the world (inactive scene)",
           WM.getAllObjects().size(), 0);
  scene->activate();
  assertEq("object is in the world (active scene)", WM.getAllObjects().size(),
           1);
}

void sceneManagement() {
  WM.startUp();

  auto scene1 = WM.createScene<Scene>("main");
  auto scene2 = WM.createScene<Scene>("other");

  assertEq("has all scenes", WM.getScenes().size(), 2);

  scene1->activate();
  const vector<unique_ptr<Scene>> &scenes = WM.getScenes();

  bool isMainActive = false;
  for (auto &scene : scenes) {
    if (scene.get() == scene1 && scene.get()->isActive()) {
      isMainActive = true;
    }
  }
  assert("activates scene", isMainActive);

  WM.switchToScene("other");
  bool isMainInactive = false;
  bool isOtherActive = false;
  for (auto &scene : scenes) {
    if (scene.get() == scene2 && scene.get()->isActive()) {
      isOtherActive = true;
    }
    if (scene.get() == scene1 && !scene.get()->isActive()) {
      isMainInactive = true;
    }
  }
  assert("switches to other scene", isMainInactive && isOtherActive);

  WM.deactivateScene("other");
  bool isOtherInactive = false;
  for (auto &scene : scenes) {
    if (scene.get() == scene2 && !scene.get()->isActive()) {
      isOtherInactive = true;
    }
  }
  assert("deactivates other scene", isOtherInactive);

  WM.shutDown();
}

auto main() -> int {
  test("object management", objectManagement);
  test("draw", draw);
  test("scene management", sceneManagement);
  return report();
}
