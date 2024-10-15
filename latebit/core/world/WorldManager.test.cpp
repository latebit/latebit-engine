#include "latebit/core/world/WorldManager.h"

#include <array>
#include <vector>

#include "../../../test/lib/test.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/objects/Object.h"
#include "latebit/core/objects/ObjectUtils.h"

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
    WM.createObject<TestObject>(scene, i);
  };
  scene->activate();

  WM.draw();
  for (int i : drawCount) {
    assertEq("draws all objects", i, 1);
  }

  WM.shutDown();
  WM.startUp();

  scene = WM.createScene<Scene>("main");
  WM.createObject<TestObject>(scene, 0, Vector(-2, -2));
  WM.draw();
  assertEq("does not draw out of bounds", drawCount[0], 1);

  WM.shutDown();
}

void getCollisions() {
  WM.startUp();

  // Create test objects
  auto scene = WM.createScene<Scene>("main");
  auto obj1 = WM.createObject<Object>(scene);
  auto obj2 = WM.createObject<Object>(scene);
  auto obj3 = WM.createObject<Object>(scene);
  auto obj4 = WM.createObject<Object>(scene);
  scene->activate();

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
  vector<Object*> collisions = WM.getCollisions(obj1, Vector(0, 0));

  // Check if obj4 is in the collisions list
  assert("collides with hard", contains(collisions, obj4));
  assert("collides with soft", contains(collisions, obj2));
  assert("does not collide with spectral", !contains(collisions, obj3));

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

  auto scene = WM.createScene<Scene>("main");
  auto subject = WM.createObject<Object>(scene, "subject");
  auto softObject = WM.createObject<Object>(scene, "soft");
  auto hardObject = WM.createObject<Object>(scene, "hard");
  auto spectralObject = WM.createObject<Object>(scene, "spectral");
  scene->activate();

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

  Vector targetPosition, targetVelocity, targetAcceleration, position, velocity,
    acceleration;

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
  assertEq("reflects velocity (subject)", subject->getVelocity(),
           targetVelocity);
  assertEq("reflects velocity (object)", hardObject->getVelocity(), velocity);
  assertEq("reflects acceleration (subject)", subject->getAcceleration(),
           targetAcceleration);
  assertEq("reflects acceleration (object)", hardObject->getAcceleration(),
           acceleration);

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
  assertEq("moves SPECTRAL on SPECTRAL", subject->getPosition(),
           targetPosition);

  targetPosition = hardObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL on HARD", subject->getPosition(), targetPosition);

  targetPosition = Vector(0, 0);
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL on empty spot", subject->getPosition(),
           targetPosition);

  subject->setSolidness(Solidness::SOFT);
  subject->setPosition(Vector(0, 0));

  targetPosition = softObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SPECTRAL over SPECTRAL", subject->getPosition(),
           targetPosition);

  targetPosition = spectralObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SOFT over SPECTRAL", subject->getPosition(), targetPosition);

  targetPosition = hardObject->getPosition();
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SOFT over SPECTRAL", subject->getPosition(), targetPosition);

  targetPosition = Vector(0, 0);
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves SOFT over empty spot", subject->getPosition(),
           targetPosition);

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
  assertEq("reflects velocity (subject)", subject->getVelocity(),
           targetVelocity);
  assertEq("reflects acceleration (subject)", subject->getAcceleration(),
           targetAcceleration);
  assertEq("reflects velocity (object)", hardObject->getVelocity(), velocity);
  assertEq("reflects acceleration (object)", hardObject->getAcceleration(),
           acceleration);

  // Clean up test objects
  WM.markForDelete(subject);
  WM.markForDelete(softObject);
  WM.markForDelete(hardObject);
  WM.update();

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

  auto scene = WM.createScene<Scene>("main");
  auto obj1 = WM.createObject<TestObject>(scene);
  scene->activate();

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

  auto scene = WM.createScene<Scene>("main");
  for (int i = 0; i < 5; i++) objects[i] = WM.createObject<Object>(scene);
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
}

auto main() -> int {
  test("object management", objectManagement);
  test("collisions ", getCollisions);
  test("resolve movements", resolveMovement);
  test("outOfBounds", outOfBounds);
  test("draw", draw);
  return report();
}
