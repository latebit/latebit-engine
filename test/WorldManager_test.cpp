#include "../include/WorldManager.h"

#include <iostream>

#include "../include/Box.h"
#include "../include/DisplayManager.h"
#include "../include/EventOut.h"
#include "../include/Vector.h"
#include "../include/utils.h"
#include "test.h"

int WorldManager_draw_test_drawCount[5];
int WorldManager_draw_test() {
  printf("WorldManager_draw_test\n");
  int result = 0;
  WM.startUp();

  class TestObject : public df::Object {
   public:
    TestObject(int altitude) { setAltitude(altitude); }
    int draw() {
      // Count the number of drawings per altitude
      WorldManager_draw_test_drawCount[getAltitude()]++;
      return df::Object::draw();
    }
  };

  // Initialize objects and results array
  for (int i = 0; i < 5; i++) {
    WorldManager_draw_test_drawCount[i] = 0;
    new TestObject(i);
  };

  WM.draw();
  for (int i = 0; i < 5; i++) {
    result +=
        assert_int("draws all objects", WorldManager_draw_test_drawCount[i], 1);
  }

  return result;
}

int WorldManager_getCollisions_test() {
  printf("WorldManager_getCollisions_test\n");
  WM.startUp();

  int result = 0;

  // Create test objects
  auto obj1 = new df::Object;
  auto obj2 = new df::Object;
  auto obj3 = new df::Object;
  auto obj4 = new df::Object;

  // Set positions of test objects
  obj1->setPosition(df::Vector(0, 0));
  obj2->setPosition(df::Vector(1, 1));
  obj3->setPosition(df::Vector(2, 2));
  obj4->setPosition(df::Vector(0, 0));

  // Set obj1 as solid
  obj1->setSolidness(df::HARD);
  obj2->setSolidness(df::SOFT);
  obj3->setSolidness(df::SPECTRAL);

  // Call getCollisions function
  df::ObjectList collisions = WM.getCollisions(obj1, df::Vector(0, 0));

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

int WorldManager_moveObject_test() {
  printf("WorldManager_moveObject_test\n");
  WM.startUp();

  int result = 0;

  // Create test objects
  auto subject = new df::Object;
  auto softObject = new df::Object;
  auto hardObject = new df::Object;
  auto spectralObject = new df::Object;

  // For debugging
  softObject->setType("soft");
  hardObject->setType("hard");
  spectralObject->setType("spectral");

  // Set positions of test objects
  subject->setPosition(df::Vector(0, 0));
  softObject->setPosition(df::Vector(2, 2));
  hardObject->setPosition(df::Vector(4, 4));
  spectralObject->setPosition(df::Vector(6, 6));

  // Set obj1 as solid
  subject->setSolidness(df::HARD);
  softObject->setSolidness(df::SOFT);
  hardObject->setSolidness(df::HARD);
  spectralObject->setSolidness(df::SPECTRAL);

  df::Vector destination = softObject->getPosition();
  result +=
      assert_ok("moves HARD over SOFT", WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  result += assert_ok("moves HARD over SPECTRAL",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  auto previousPosition = subject->getPosition();
  destination = hardObject->getPosition();
  result += assert_fail("does not move HARD over HARD",
                        WM.moveObject(subject, destination));
  result += assert_vector("does not update position", subject->getPosition(),
                          previousPosition);

  destination = df::Vector(0, 0);
  result += assert_ok("moves HARD on empty spot",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(df::SPECTRAL);
  subject->setPosition(df::Vector(0, 0));

  destination = softObject->getPosition();
  result += assert_ok("moves SPECTRAL over SOFT",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  result += assert_ok("moves SPECTRAL over SPECTRAL",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = hardObject->getPosition();
  result += assert_ok("moves SPECTRAL over HARD",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = df::Vector(0, 0);
  result += assert_ok("moves SPECTRAL on empty spot",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(df::SOFT);
  subject->setPosition(df::Vector(0, 0));

  destination = softObject->getPosition();
  result +=
      assert_ok("moves SOFT over SOFT", WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  result += assert_ok("moves SOFT over SPECTRAL",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = hardObject->getPosition();
  result +=
      assert_ok("moves SOFT over HARD", WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = df::Vector(0, 0);
  result += assert_ok("moves SOFT on empty spot",
                      WM.moveObject(subject, destination));
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(df::HARD);
  subject->setBox(df::Box(df::Vector(), 1.5, 1.5));
  // Almost on hard, but with part of the bounding box colliding
  destination = hardObject->getPosition() - df::Vector(1, 1);
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

bool WorldManager_outOfBounds_test_emitted = false;
int WorldManager_outOfBounds_test() {
  printf("WorldManager_outOfBounds_test\n");
  WM.startUp();
  int result = 0;

  class TestObject : public df::Object {
   public:
    int eventHandler(const df::Event* p_e) {
      if (p_e->getType() == df::OUT_EVENT) {
        WorldManager_outOfBounds_test_emitted = true;
        return 1;
      }
      return 0;
    }
  };

  auto obj1 = new TestObject;

  obj1->setPosition(df::Vector(0, 0));

  WM.moveObject(obj1, df::Vector(-1, 0));
  result += assert("emits out of bounds event",
                   WorldManager_outOfBounds_test_emitted);

  WorldManager_outOfBounds_test_emitted = false;
  WM.moveObject(obj1, df::Vector(-2, 0));
  result += assert("does not emit out of bounds if already out",
                   !WorldManager_outOfBounds_test_emitted);

  WM.shutDown();
  return result;
}

int WorldManager_test() {
  int result = 0;

  auto obj1 = new df::Object;
  auto obj2 = new df::Object;
  auto obj3 = new df::Object;
  auto obj4 = new df::Object;

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

  return result;
}
