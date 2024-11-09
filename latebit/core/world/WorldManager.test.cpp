#include "WorldManager.h"

#include <array>
#include <memory>
#include <vector>

#include "../../../test/lib/test.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/ObjectUtils.h"

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

void getCollisions() {
  WM.startUp();

  auto scene = WM.createScene<Scene>("main");
  auto obj1 = scene->createObject<Object>();
  auto obj2 = scene->createObject<Object>();
  auto obj3 = scene->createObject<Object>();
  auto obj4 = scene->createObject<Object>();
  scene->activate();

  // Objects need a non-zero box to collide
  obj1->setPosition(Vector(0, 0));
  obj1->setBox({1, 1});
  obj2->setPosition(Vector(1, 1));
  obj2->setBox({1, 1});
  obj3->setPosition(Vector(2, 2));
  obj3->setBox({1, 1});
  obj4->setPosition(Vector(0, 0));
  obj4->setBox({1, 1});

  obj1->setBodyType(BodyType::DYNAMIC);
  obj2->setBodyType(BodyType::TRIGGER);
  obj3->setBodyType(BodyType::KINEMATIC);
  obj4->setBodyType(BodyType::DYNAMIC);

  vector<Object *> collisions = WM.getCollisions(obj1, Vector(0, 0));

  assert("collides with hard", contains(collisions, obj4));
  assert("collides with soft", contains(collisions, obj2));
  assert("does not collide with spectral", !contains(collisions, obj3));

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
  auto subject = scene->createObject<Object>("subject");
  auto softObject = scene->createObject<Object>("soft");
  auto hardObject = scene->createObject<Object>("hard");
  auto spectralObject = scene->createObject<Object>("spectral");
  scene->activate();

  subject->setPosition(Vector(0, 0));
  softObject->setPosition(Vector(2, 2));
  hardObject->setPosition(Vector(4, 4));
  spectralObject->setPosition(Vector(6, 6));

  subject->setBodyType(BodyType::DYNAMIC);
  softObject->setBodyType(BodyType::TRIGGER);
  hardObject->setBodyType(BodyType::DYNAMIC);
  spectralObject->setBodyType(BodyType::KINEMATIC);

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
  assertEq("same velocity (subject)", subject->getVelocity(),
           velocity);
  assertEq("same velocity (object)", hardObject->getVelocity(), targetVelocity);

  targetPosition = Vector(0, 0);
  WM.resolveMovement(subject, targetPosition);
  assertEq("moves HARD on empty spot", subject->getPosition(), targetPosition);

  subject->setBodyType(BodyType::KINEMATIC);
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

  subject->setBodyType(BodyType::TRIGGER);
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

  subject->setBodyType(BodyType::DYNAMIC);
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
  assertEq("reflects velocity (object)", hardObject->getVelocity(), velocity);

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
    auto eventHandler(const Event *e) -> int override {
      if (e->getType() == OUT_EVENT) {
        outOfBounds_emitted = true;
        return 1;
      }
      return 0;
    }
  };

  auto scene = WM.createScene<Scene>("main");
  auto obj1 = scene->createObject<TestObject>();
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
  test("collisions ", getCollisions);
  test("resolve movements", resolveMovement);
  test("outOfBounds", outOfBounds);
  test("draw", draw);
  test("scene management", sceneManagement);
  return report();
}
