#include "../include/WorldManager.h"

#include <array>
#include <iostream>

#include "../include/Box.h"
#include "../include/DisplayManager.h"
#include "../include/EventOut.h"
#include "../include/Vector.h"
#include "../include/utils.h"
#include "test.h"

using namespace std;
using namespace df;

array<int, 5> WorldManager_draw_test_drawCount;
auto WorldManager_draw_test() -> int {
  printf("WorldManager_draw_test\n");
  int result = 0;
  WM.startUp();

  class TestObject : public Object {
   public:
    TestObject(int altitude, Vector position = Vector()) {
      setAltitude(altitude);
      setPosition(position);
    }
    auto draw() -> int override {
      // Count the number of drawings per altitude
      WorldManager_draw_test_drawCount[getAltitude()]++;
      return Object::draw();
    }
  };

  // Initialize objects and results array
  for (int i = 0; i < 5; i++) {
    WorldManager_draw_test_drawCount[i] = 0;
    new TestObject(i);
  };

  WM.draw();
  for (int i : WorldManager_draw_test_drawCount) {
    result += assert_int("draws all objects", i, 1);
  }

  WM.shutDown();
  WM.startUp();

  auto obj1 = new TestObject(0, Vector(-2, -2));
  WM.draw();
  result += assert_int("does not draw out of bounds",
                       WorldManager_draw_test_drawCount[0], 1);

  WM.shutDown();

  return result;
}

auto WorldManager_getCollisions_test() -> int {
  printf("WorldManager_getCollisions_test\n");
  WM.startUp();

  int result = 0;

  // Create test objects
  auto obj1 = new Object;
  auto obj2 = new Object;
  auto obj3 = new Object;
  auto obj4 = new Object;

  // Set positions of test objects
  obj1->setPosition(Vector(0, 0));
  obj2->setPosition(Vector(1, 1));
  obj3->setPosition(Vector(2, 2));
  obj4->setPosition(Vector(0, 0));

  // Set obj1 as solid
  obj1->setSolidness(HARD);
  obj2->setSolidness(SOFT);
  obj3->setSolidness(SPECTRAL);

  // Call getCollisions function
  ObjectList collisions = WM.getCollisions(obj1, Vector(0, 0));

  // Check if obj4 is in the collisions list
  result += assert("collides with hard", collisions.find(obj4) > -1);
  result += assert("collides with soft", collisions.find(obj2) > -1);
  result +=
    assert("does not collide with spectral", collisions.find(obj3) == -1);

  // Clean up test objects
  WM.markForDelete(obj1);
  WM.markForDelete(obj2);
  WM.markForDelete(obj3);
  WM.markForDelete(obj4);
  WM.update();

  WM.shutDown();
  return result;
}

auto WorldManager_moveObject_test() -> int {
  printf("WorldManager_moveObject_test\n");
  WM.startUp();

  int result = 0;

  // Create test objects
  auto subject = new Object;
  auto softObject = new Object;
  auto hardObject = new Object;
  auto spectralObject = new Object;

  // For debugging
  softObject->setType("soft");
  hardObject->setType("hard");
  spectralObject->setType("spectral");

  // Set positions of test objects
  subject->setPosition(Vector(0, 0));
  softObject->setPosition(Vector(2, 2));
  hardObject->setPosition(Vector(4, 4));
  spectralObject->setPosition(Vector(6, 6));

  // Set obj1 as solid
  subject->setSolidness(HARD);
  softObject->setSolidness(SOFT);
  hardObject->setSolidness(HARD);
  spectralObject->setSolidness(SPECTRAL);

  Vector destination = softObject->getPosition();
  result +=
    assert_ok("moves HARD over SOFT", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  result +=
    assert_ok("moves HARD over SPECTRAL", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  auto previousPosition = subject->getPosition();
  destination = hardObject->getPosition();
  result += assert_fail("does not move HARD over HARD",
                        WM.moveObject(subject, destination));
  result += assert_vector("does not update position", subject->getPosition(),
                          previousPosition);

  destination = Vector(0, 0);
  result +=
    assert_ok("moves HARD on empty spot", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(SPECTRAL);
  subject->setPosition(Vector(0, 0));

  destination = softObject->getPosition();
  result +=
    assert_ok("moves SPECTRAL over SOFT", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  result += assert_ok("moves SPECTRAL over SPECTRAL",
                      WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = hardObject->getPosition();
  result +=
    assert_ok("moves SPECTRAL over HARD", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = Vector(0, 0);
  result += assert_ok("moves SPECTRAL on empty spot",
                      WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(SOFT);
  subject->setPosition(Vector(0, 0));

  destination = softObject->getPosition();
  result +=
    assert_ok("moves SOFT over SOFT", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  result +=
    assert_ok("moves SOFT over SPECTRAL", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = hardObject->getPosition();
  result +=
    assert_ok("moves SOFT over HARD", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  destination = Vector(0, 0);
  result +=
    assert_ok("moves SOFT on empty spot", WM.moveObject(subject, destination));
  result +=
    assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(HARD);
  subject->setBox(Box(Vector(), 1.5, 1.5));
  // Almost on hard, but with part of the bounding box colliding
  destination = hardObject->getPosition() - Vector(1, 1);
  result += assert_fail("does not move HARD over HARD (larger bounding boxes)",
                        WM.moveObject(subject, destination));

  // Clean up test objects
  WM.markForDelete(subject);
  WM.markForDelete(softObject);
  WM.markForDelete(hardObject);
  WM.update();

  WM.shutDown();
  return result;
}

auto WorldManager_viewFollowing_test() -> int {
  printf("WorldManager_viewFollowing_test\n");
  int result = 0;
  WM.startUp();

  auto subject = new Object;
  subject->setPosition(Vector());

  auto initialView = Box(Vector(5, 5), 10, 10);
  WM.setView(initialView);
  WM.setBoundary(Box(20, 20));

  WM.setViewFollowing(subject);
  WM.moveObject(subject, Vector(10, 10));
  result += assert_box("does not update view", WM.getView(), initialView);
  WM.moveObject(subject, Vector(11, 11));
  result +=
    assert_box("updates the view", WM.getView(), Box(Vector(6, 6), 10, 10));

  WM.moveObject(subject, Vector(11, 5));
  result += assert_box("updates the view (vertical lower bound)", WM.getView(),
                       Box(Vector(6, 0), 10, 10));

  WM.moveObject(subject, Vector(11, 15));
  result += assert_box("updates the view (vertical upper bound)", WM.getView(),
                       Box(Vector(6, 10), 10, 10));

  WM.moveObject(subject, Vector(5, 11));
  result += assert_box("updates the view (horizontal lower bound)",
                       WM.getView(), Box(Vector(0, 6), 10, 10));

  WM.moveObject(subject, Vector(15, 11));
  result += assert_box("updates the view (horizontal upper bound)",
                       WM.getView(), Box(Vector(10, 6), 10, 10));

  WM.shutDown();
  return result;
}

bool WorldManager_outOfBounds_test_emitted = false;
auto WorldManager_outOfBounds_test() -> int {
  printf("WorldManager_outOfBounds_test\n");
  WM.startUp();
  int result = 0;

  class TestObject : public Object {
   public:
    auto eventHandler(const Event* e) -> int override {
      if (e->getType() == OUT_EVENT) {
        WorldManager_outOfBounds_test_emitted = true;
        return 1;
      }
      return 0;
    }
  };

  auto obj1 = new TestObject;

  obj1->setPosition(Vector());
  obj1->setBox(Box(1, 1));

  WM.moveObject(obj1, Vector(-2, 0));
  result +=
    assert("emits out of bounds event", WorldManager_outOfBounds_test_emitted);

  WorldManager_outOfBounds_test_emitted = false;
  WM.moveObject(obj1, Vector(-3, 0));
  result += assert("does not emit out of bounds if already out",
                   !WorldManager_outOfBounds_test_emitted);

  WM.shutDown();
  return result;
}

auto WorldManager_test() -> int {
  int result = 0;

  // setters
  WM.setView(Box(Vector(0, 0), 10, 10));
  result += assert_box("sets view", WM.getView(), Box(Vector(0, 0), 10, 10));

  WM.setBoundary(Box(Vector(0, 0), 10, 10));
  result +=
    assert_box("sets boundary", WM.getBoundary(), Box(Vector(0, 0), 10, 10));

  auto obj1 = new Object;
  auto obj2 = new Object;
  auto obj3 = new Object;
  auto obj4 = new Object;

  obj3->setType("type");
  obj4->setType("type");

  WM.startUp();

  WM.insertObject(obj1);
  WM.insertObject(obj2);
  WM.insertObject(obj3);
  WM.insertObject(obj4);
  WM.update();

  result += assert_int("has all the objects", WM.getAllObjects().getCount(), 4);
  result += assert_int("filters objects by type",
                       WM.objectsOfType("type").getCount(), 2);

  WM.markForDelete(obj1);
  WM.update();
  result += assert_int("has one less object", WM.getAllObjects().getCount(), 3);

  WM.removeObject(obj2);
  WM.update();
  result += assert_int("removes an object", WM.getAllObjects().getCount(), 2);

  WM.shutDown();
  result += assert_int("removes everything", WM.getAllObjects().getCount(), 0);

  result += WorldManager_getCollisions_test();
  result += WorldManager_moveObject_test();
  result += WorldManager_outOfBounds_test();
  result += WorldManager_draw_test();
  result += WorldManager_viewFollowing_test();

  return result;
}
