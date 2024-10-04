#include "latebit/core/world/WorldManager.h"

#include <array>

#include "../../../test/lib/test.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/Object.h"

using namespace std;
using namespace lb;

void worldToView() {
  auto initialView = WM.getView();
  WM.setView(Box(Vector(5, 5), 10, 10));

  assertEq("converts world position to view position",
           WorldManager::worldToView(Vector(5, 5)), Vector(0, 0));
  assertEq("converts world position to view position (origin)",
           WorldManager::worldToView(Vector(0, 0)), Vector(-5, -5));
  WM.setView(initialView);
}

void viewToWorld() {
  auto initialView = WM.getView();
  WM.setView(Box(Vector(5, 5), 10, 10));

  assertEq("converts view position to world position",
           WorldManager::viewToWorld(Vector(0, 0)), Vector(5, 5));
  assertEq("converts view position to world position (origin)",
           WorldManager::worldToView(Vector(-5, -5)), Vector(0, 0));
  WM.setView(initialView);
}

void draw() {
  WM.startUp();
  static array<int, 5> drawCount;

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
  for (int i = 0; i < 5; i++) {
    drawCount[i] = 0;
    new TestObject(i);
  };

  WM.draw();
  for (int i : drawCount) {
    assertEq("draws all objects", i, 1);
  }

  WM.shutDown();
  WM.startUp();

  new TestObject(0, Vector(-2, -2));
  WM.draw();
  assertEq("does not draw out of bounds", drawCount[0], 1);

  WM.shutDown();
}

void getCollisions() {
  WM.startUp();

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
  obj1->setSolidness(Solidness::HARD);
  obj2->setSolidness(Solidness::SOFT);
  obj3->setSolidness(Solidness::SPECTRAL);

  // Call getCollisions function
  ObjectList collisions = WM.getCollisions(obj1, Vector(0, 0));

  // Check if obj4 is in the collisions list
  assert("collides with hard", collisions.find(obj4) > -1);
  assert("collides with soft", collisions.find(obj2) > -1);

  assert("does not collide with spectral", collisions.find(obj3) == -1);

  // Clean up test objects
  WM.markForDelete(obj1);
  WM.markForDelete(obj2);
  WM.markForDelete(obj3);
  WM.markForDelete(obj4);
  WM.update();

  WM.shutDown();
}

void resolveMovement() {
  WM.startUp();

  auto subject = new Object("subject");
  auto softObject = new Object("soft");
  auto hardObject = new Object("hard");
  auto spectralObject = new Object("spectral");

  subject->setPosition(Vector(0, 0));
  softObject->setPosition(Vector(2, 2));
  hardObject->setPosition(Vector(4, 4));
  spectralObject->setPosition(Vector(6, 6));

  subject->setSolidness(Solidness::HARD);
  softObject->setSolidness(Solidness::SOFT);
  hardObject->setSolidness(Solidness::HARD);
  spectralObject->setSolidness(Solidness::SPECTRAL);

  subject->setVelocity(Vector(1, 1));
  softObject->setVelocity(Vector(-1, -1));
  hardObject->setVelocity(Vector(-1, -1));
  spectralObject->setVelocity(Vector(-1, -1));

  Vector targetPosition, targetVelocity, targetAcceleration, position, velocity, acceleration;
  
  targetPosition = softObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves HARD on SOFT", subject->getPosition(), targetPosition);

  targetPosition = spectralObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves HARD on SPECTRAL", subject->getPosition(), targetPosition);

  position = subject->getPosition();
  velocity = subject->getVelocity();
  acceleration = subject->getAcceleration();

  targetPosition = hardObject->getPosition();
  targetVelocity = hardObject->getVelocity();
  targetAcceleration = hardObject->getAcceleration();

  WM.resolveMovement(subject, targetPosition);
  assertEq("does not move HARD over HARD", subject->getPosition(), position);
  assertEq("reflects velocity (subject)", subject->getVelocity(), targetVelocity);
  assertEq("reflects velocity (object)", hardObject->getVelocity(), velocity);
  assertEq("reflects acceleration (subject)", subject->getAcceleration(),
           targetAcceleration);
  assertEq("reflects acceleration (object)", hardObject->getAcceleration(), acceleration);

  targetPosition = Vector(0, 0);
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves HARD on empty spot", subject->getPosition(), targetPosition);

  subject->setSolidness(Solidness::SPECTRAL);
  subject->setPosition(Vector(0, 0));

  targetPosition = softObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL on SOFT", subject->getPosition(), targetPosition);

  targetPosition = spectralObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL on SPECTRAL", subject->getPosition(), targetPosition);

  targetPosition = hardObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL on HARD", subject->getPosition(), targetPosition);

  targetPosition = Vector(0, 0);
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL on empty spot", subject->getPosition(), targetPosition);

  subject->setSolidness(Solidness::SOFT);
  subject->setPosition(Vector(0, 0));

  targetPosition = softObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL over SPECTRAL", subject->getPosition(), targetPosition);

  targetPosition = spectralObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SOFT over SPECTRAL", subject->getPosition(), targetPosition);

  targetPosition = hardObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SOFT over SPECTRAL", subject->getPosition(), targetPosition);

  targetPosition = Vector(0, 0);
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SOFT over empty spot", subject->getPosition(), targetPosition);

  position = subject->getPosition();
  velocity = Vector(1, 1);
  acceleration = Vector(2, 2);
  targetVelocity = Vector(-1, -1);
  targetAcceleration = Vector(-2, -2);

  subject->setSolidness(Solidness::HARD);
  subject->setBox(Box(Vector(), 1.5, 1.5));
  subject->setVelocity(velocity);
  subject->setAcceleration(acceleration);
  hardObject->setVelocity(targetVelocity);
  hardObject->setAcceleration(targetAcceleration);
  
  // Almost on hard, but with part of the bounding box colliding
  targetPosition = hardObject->getPosition() - velocity;
  WM.resolveMovement(subject, targetPosition);
  assertEq("does not move HARD on HARD with bounding box collision",
           subject->getPosition(), position);
  assertEq("reflects velocity (subject)", subject->getVelocity(), targetVelocity);
  assertEq("reflects acceleration (subject)", subject->getAcceleration(), targetAcceleration);
  assertEq("reflects velocity (object)", hardObject->getVelocity(), velocity);
  assertEq("reflects acceleration (object)", hardObject->getAcceleration(), acceleration);

  // Clean up test objects
  WM.markForDelete(subject);
  WM.markForDelete(softObject);
  WM.markForDelete(hardObject);
  WM.update();

  WM.shutDown();
}

void viewFollowing() {
  WM.startUp();

  auto subject = new Object;
  subject->setPosition(Vector());

  auto initialView = Box(Vector(5, 5), 10, 10);
  WM.setView(initialView);
  WM.setBoundary(Box(20, 20));

  WM.setViewFollowing(subject);
  WM.resolveMovement(subject, Vector(10, 10));
  assertEq("does not update view", WM.getView(), initialView);
  WM.resolveMovement(subject, Vector(11, 11));

  assertEq("updates the view", WM.getView(), Box(Vector(6, 6), 10, 10));

  WM.resolveMovement(subject, Vector(11, 5));
  assertEq("updates the view (vertical lower bound)", WM.getView(),
           Box(Vector(6, 0), 10, 10));

  WM.resolveMovement(subject, Vector(11, 15));
  assertEq("updates the view (vertical upper bound)", WM.getView(),
           Box(Vector(6, 10), 10, 10));

  WM.resolveMovement(subject, Vector(5, 11));
  assertEq("updates the view (horizontal lower bound)", WM.getView(),
           Box(Vector(0, 6), 10, 10));

  WM.resolveMovement(subject, Vector(15, 11));
  assertEq("updates the view (horizontal upper bound)", WM.getView(),
           Box(Vector(10, 6), 10, 10));

  WM.setViewDeadZone(Box(WM.getView().getCorner(), 5, 5));
  WM.resolveMovement(subject, Vector(12, 10));
  assertEq("does not update the view within dead zone", WM.getView(),
           Box(Vector(10, 6), 10, 10));

  WM.shutDown();
}

bool outOfBounds_emitted = false;
void outOfBounds() {
  WM.startUp();

  class TestObject : public Object {
   public:
    auto eventHandler(const Event* e) -> int override {
      if (e->getType() == OUT_EVENT) {
        outOfBounds_emitted = true;
        return 1;
      }
      return 0;
    }
  };

  auto obj1 = new TestObject;

  obj1->setPosition(Vector());
  obj1->setBox(Box(1, 1));

  WM.resolveMovement(obj1, Vector(-2, 0));

  assert("emits out of bounds event", outOfBounds_emitted);

  outOfBounds_emitted = false;
  WM.resolveMovement(obj1, Vector(-3, 0));
  assert("does not emit out of bounds if already out", !outOfBounds_emitted);

  WM.shutDown();
}

void objectManagement() {
  WM.startUp();
  array<Object*, 5> objects;

  for (int i = 0; i < 5; i++) objects[i] = new Object;

  objects[0]->setType("lol");
  objects[1]->setType("asd");
  objects[2]->setType("type");
  objects[3]->setType("type");
  objects[4]->setType("type");
  objects[4]->setActive(false);

  WM.update();

  auto activeObjects = WM.getAllObjects();
  assertEq("has active objects", activeObjects.getCount(), 4);
  auto allObjects = WM.getAllObjects(true);
  assertEq("has all the objects", allObjects.getCount(), 5);
  auto activeTypeObjects = WM.getAllObjectsByType("type");
  assertEq("filters active objects by type", activeTypeObjects.getCount(), 2);
  auto allTypeObjects = WM.getAllObjectsByType("type", true);
  assertEq("filters all objects by type", allTypeObjects.getCount(), 3);

  WM.markForDelete(objects[0]);
  WM.update();
  activeObjects = WM.getAllObjects();
  assertEq("has one less object", activeObjects.getCount(), 3);

  WM.removeObject(objects[1]);
  WM.update();
  activeObjects = WM.getAllObjects();
  assertEq("removes an object", activeObjects.getCount(), 2);
  delete (objects[1]);

  WM.shutDown();
  activeObjects = WM.getAllObjects();
  assertEq("removes everything", activeObjects.getCount(), 0);
}

void setters() {
  WM.setView(Box(Vector(0, 0), 10, 10));
  assertEq("sets view", WM.getView(), Box(Vector(0, 0), 10, 10));

  WM.setBoundary(Box(Vector(0, 0), 10, 10));
  assertEq("sets boundary", WM.getBoundary(), Box(Vector(0, 0), 10, 10));

  WM.setViewDeadZone(Box(Vector(0, 0), 10, 10));
  assertEq("sets view dead zone", WM.getViewDeadZone(),
           Box(Vector(0, 0), 10, 10));

  WM.setView(Box(Vector(0, 0), 10, 10));
  assertEq("sets view", WM.getView(), Box(Vector(0, 0), 10, 10));
}

auto main() -> int {
  test("setters", setters);
  test("object management", objectManagement);
  test("collisions ", getCollisions);
  test("resolve movements", resolveMovement);
  test("outOfBounds", outOfBounds);
  test("draw", draw);
  test("viewFollowing", viewFollowing);
  test("worldToView", worldToView);
  test("viewToWorld", worldToView);
  return report();
}
