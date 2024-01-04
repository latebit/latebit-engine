#include "../src/WorldManager.h"

#include <iostream>

#include "../src/Vector.h"
#include "test.h"

int WorldManager_getCollisions_test() {
  std::cout << "WorldManager_getCollisions_test" << std::endl;

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

  return result;
}

int WorldManager_moveObject_test() {
  std::cout << "WorldManager_moveObject_test" << std::endl;

  int result = 0;

  // Create test objects
  auto subject = new df::Object;
  auto softObject = new df::Object;
  auto hardObject = new df::Object;
  auto spectralObject = new df::Object;

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
  int moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves HARD over SOFT", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves HARD over SPECTRAL", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  auto previousPosition = subject->getPosition();
  destination = hardObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("does not move HARD over HARD", moveResult, -1);
  result += assert_vector("does not update position", subject->getPosition(),
                          previousPosition);

  destination = df::Vector(0, 0);
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves HARD on empty spot", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(df::SPECTRAL);
  subject->setPosition(df::Vector(0, 0));

  destination = softObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SPECTRAL over SOFT", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SPECTRAL over SPECTRAL", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = hardObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SPECTRAL over HARD", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = df::Vector(0, 0);
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SPECTRAL on empty spot", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  subject->setSolidness(df::SOFT);
  subject->setPosition(df::Vector(0, 0));

  destination = softObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SOFT over SOFT", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = spectralObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SOFT over SPECTRAL", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = hardObject->getPosition();
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SOFT over HARD", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  destination = df::Vector(0, 0);
  moveResult = WM.moveObject(subject, destination);
  result += assert_int("moves SOFT on empty spot", moveResult, 0);
  result +=
      assert_vector("updates position", subject->getPosition(), destination);

  // Clean up test objects
  WM.markForDelete(subject);
  WM.markForDelete(softObject);
  WM.markForDelete(hardObject);
  WM.update();

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

  WM.markForDelete(obj1);
  WM.markForDelete(obj2);
  WM.markForDelete(obj3);
  WM.markForDelete(obj4);

  result += WorldManager_getCollisions_test();
  result += WorldManager_moveObject_test();

  return result;
}
