#include "latebit/core/world/Physics.h"

#include "../../../test/lib/test.h"
#include "latebit/core/events/EventCollision.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/events/EventOverlap.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/ObjectUtils.h"
#include "latebit/core/world/Scene.h"
#include "latebit/core/world/WorldManager.h"

using namespace lb;

void reset(Object *o) {
  // Just get it out of the way
  o->setPosition(Vector(-10, -10));
  o->setVelocity(Vector());
  o->setAcceleration(Vector());
}

void overlaps() {
  WM.startUp();

  auto scene = WM.createScene<Scene>("main");
  auto obj1 = scene->createObject<Object>();
  auto obj2 = scene->createObject<Object>();
  auto obj3 = scene->createObject<Object>();
  auto obj4 = scene->createObject<Object>();
  scene->activate();

  obj1->setSolidness(Solidness::HARD);
  obj2->setSolidness(Solidness::SOFT);
  obj3->setSolidness(Solidness::SPECTRAL);
  obj4->setSolidness(Solidness::HARD);

  Physics physics = WM.getPhysics();
  vector<Object *> collisions = physics.getOverlapsAt(obj1, Vector(0, 0));

  assert("collides with hard", contains(collisions, obj4));
  assert("collides with soft", contains(collisions, obj2));
  assert("does not collide with spectral", !contains(collisions, obj3));

  WM.shutDown();
}

void outOfBounds() {
  WM.startUp();
  static int emitted = 0;

  class TestObject : public Object {
   public:
    auto eventHandler(const Event *e) -> int override {
      if (e->getType() == OUT_EVENT) {
        emitted++;
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

  Physics physics(&WM.getSceneGraph(), &WM);
  physics.moveTo(obj1, Vector(-2, -2));

  assertEq("emits out of bounds event", emitted, 1);

  physics.moveTo(obj1, Vector(-4, -4));
  assertEq("does not emit out of bounds if already out", emitted, 1);

  WM.shutDown();
}

void overlapResponse() {
  WM.startUp();
  auto &physics = WM.getPhysics();

  auto scene = WM.createScene<Scene>("main");
  auto subject = scene->createObject<Object>("subject");
  auto soft = scene->createObject<Object>("soft");
  auto hard = scene->createObject<Object>("hard");
  auto spectral = scene->createObject<Object>("spectral");
  scene->activate();

  subject->setSolidness(Solidness::HARD);
  subject->setBox({1, 1});
  soft->setSolidness(Solidness::SOFT);
  soft->setBox({1, 1});
  hard->setSolidness(Solidness::HARD);
  hard->setBox({1, 1});
  spectral->setSolidness(Solidness::SPECTRAL);
  spectral->setBox({1, 1});

  test("when subject is hard", [&]() {
    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    soft->setPosition(Vector(2, 2));
    soft->setVelocity(Vector(-1, -1));
    physics.update();
    assertEq("moves HARD on SOFT", subject->getPosition(), soft->getPosition());
    reset(subject);
    reset(soft);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    spectral->setPosition(Vector(2, 2));
    spectral->setVelocity(Vector(-1, -1));
    physics.update();
    assertEq("moves HARD on SPECTRAL", subject->getPosition(),
             spectral->getPosition());
    reset(subject);
    reset(spectral);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    hard->setPosition(Vector(1, 1));
    hard->setVelocity(Vector(0, 0));
    physics.update();
    assertEq("pushes HARD next to HARD", subject->getPosition(), {1, 2});
    reset(subject);
    reset(hard);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    physics.update();
    assertEq("moves HARD on empty spot", subject->getPosition(), {1, 1});
    reset(subject);
  });

  test("when subject is spectral", [&]() {
    subject->setSolidness(Solidness::SPECTRAL);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    soft->setPosition(Vector(2, 2));
    soft->setVelocity(Vector(-1, -1));
    physics.update();
    assertEq("moves SPECTRAL on SOFT", subject->getPosition(),
             soft->getPosition());
    reset(subject);
    reset(soft);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    spectral->setPosition(Vector(2, 2));
    spectral->setVelocity(Vector(-1, -1));
    physics.update();
    assertEq("moves SPECTRAL on SPECTRAL", subject->getPosition(),
             spectral->getPosition());
    reset(subject);
    reset(spectral);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    hard->setPosition(Vector(1, 1));
    hard->setVelocity(Vector(0, 0));
    physics.update();
    assertEq("moves SPECTRAL on HARD", subject->getPosition(), {1, 1});
    reset(subject);
    reset(hard);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    physics.update();
    assertEq("moves SPECTRAL on empty spot", subject->getPosition(), {1, 1});
    reset(subject);
  });

  test("when subject is soft", [&]() {
    subject->setSolidness(Solidness::SOFT);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    soft->setPosition(Vector(2, 2));
    soft->setVelocity(Vector(-1, -1));
    physics.update();
    assertEq("moves SOFT on SOFT", subject->getPosition(), soft->getPosition());
    reset(subject);
    reset(soft);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    spectral->setPosition(Vector(2, 2));
    spectral->setVelocity(Vector(-1, -1));
    physics.update();
    assertEq("moves SOFT on SPECTRAL", subject->getPosition(),
             spectral->getPosition());
    reset(subject);
    reset(spectral);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    hard->setPosition(Vector(1, 1));
    hard->setVelocity(Vector(0, 0));
    physics.update();
    assertEq("moves SOFT on HARD", subject->getPosition(), hard->getPosition());
    reset(subject);
    reset(hard);

    subject->setPosition(Vector(0, 0));
    subject->setVelocity(Vector(1, 1));
    physics.update();
    assertEq("moves SOFT on empty spot", subject->getPosition(), {1, 1});
    reset(subject);
  });

  WM.shutDown();
}

void kinematics() {
  WM.startUp();
  auto &physics = WM.getPhysics();

  auto scene = WM.createScene<Scene>("main");
  auto subject = scene->createObject<Object>("subject");
  auto wall = scene->createObject<Object>("wall");
  scene->activate();

  subject->setSolidness(Solidness::HARD);
  subject->setPosition(Vector(0, 0));
  subject->setVelocity(Vector(1, 1));

  physics.update();
  assertEq("moves", subject->getPosition(), Vector(1, 1));

  subject->setAcceleration(Vector(0, 0.1));
  physics.update();
  assertEq("accelerates", subject->getVelocity(), Vector(1, 1.1));

  subject->setPosition({});
  subject->setVelocity(Vector(1, 1));
  wall->setSolidness(Solidness::HARD);
  wall->setPosition(Vector(0, 1));
  physics.update();

  WM.shutDown();
}

void events() {
  WM.startUp();
  auto &physics = WM.getPhysics();

  class TestObject : public Object {
   public:
    int collisions = 0;
    int overlaps = 0;

    TestObject(Solidness::Solidness s) {
      this->setSolidness(s);
      subscribe(COLLISION_EVENT);
      subscribe(OVERLAP_EVENT);
    }

    auto eventHandler(const Event *e) -> int override {
      if (e->getType() == COLLISION_EVENT) {
        collisions++;
      }
      if (e->getType() == OVERLAP_EVENT) {
        overlaps++;
      }
      return 0;
    }

    void reset() {
      collisions = 0;
      overlaps = 0;
      ::reset(this);
    }
  };

  auto scene = WM.createScene<Scene>("main");
  auto hard = scene->createObject<TestObject>(Solidness::HARD);
  auto soft = scene->createObject<TestObject>(Solidness::SOFT);
  auto spectral = scene->createObject<TestObject>(Solidness::SPECTRAL);
  auto anotherHard = scene->createObject<TestObject>(Solidness::HARD);
  auto anotherSoft = scene->createObject<TestObject>(Solidness::SOFT);
  auto anotherSpectral = scene->createObject<TestObject>(Solidness::SPECTRAL);
  scene->activate();

  test("overlap hard with soft", [&]() {
    hard->setPosition(Vector(0, 0));
    hard->setVelocity(Vector(1, 0));
    soft->setPosition(Vector(1, 0));
    physics.update();
    assertEq("hard receives overlap event", hard->overlaps, 1);
    assertEq("hard does not receive collision event", hard->collisions, 0);
    assertEq("soft receives overlap event", soft->overlaps, 1);
    assertEq("soft does not receive collision event", soft->collisions, 0);
    hard->reset();
    soft->reset();
  });

  test("overlap hard with spectral", [&]() {
    hard->setPosition(Vector(0, 0));
    hard->setVelocity(Vector(1, 0));
    spectral->setPosition(Vector(1, 0));
    physics.update();
    assertEq("hard does not receive overlap event", hard->overlaps, 0);
    assertEq("hard does not receive collision event", hard->collisions, 0);
    assertEq("spectral does not receive overlap event", spectral->overlaps, 0);
    assertEq("spectral does not receive collision event", spectral->collisions,
             0);
    hard->reset();
    spectral->reset();
  });

  test("overlap soft with spectral", [&]() {
    soft->setPosition(Vector(0, 0));
    soft->setVelocity(Vector(1, 0));
    spectral->setPosition(Vector(1, 0));
    physics.update();
    assertEq("soft does not receive overlap event", soft->overlaps, 0);
    assertEq("soft does not receive collision event", soft->collisions, 0);
    assertEq("spectral does not receive overlap event", spectral->overlaps, 0);
    assertEq("spectral does not receive collision event", spectral->collisions,
             0);
    soft->reset();
    spectral->reset();
  });

  test("overlap hard with hard", [&]() {
    hard->setPosition(Vector(0, 0));
    hard->setVelocity(Vector(1, 0));
    anotherHard->setPosition(Vector(1, 0));
    physics.update();
    assertEq("hard receives overlap event", hard->overlaps, 1);
    assertEq("other receives overlap event", anotherHard->overlaps, 1);
    assertEq("hard receives collision event", hard->collisions, 1);
    assertEq("other hard receives collision event", anotherHard->collisions, 1);
    hard->reset();
    anotherHard->reset();
  });

  test("overlap soft with soft", [&]() {
    soft->setPosition(Vector(0, 0));
    soft->setVelocity(Vector(1, 0));
    anotherSoft->setPosition(Vector(1, 0));
    physics.update();
    assertEq("soft receives overlap event", soft->overlaps, 1);
    assertEq("other receives overlap event", anotherSoft->overlaps, 1);
    assertEq("soft does not receive collision event", soft->collisions, 0);
    assertEq("other soft does not receive collision event",
             anotherSoft->collisions, 0);
    soft->reset();
    anotherSoft->reset();
  });

  test("overlap spectral with spectral", [&]() {
    spectral->setPosition(Vector(0, 0));
    anotherSpectral->setPosition(Vector(1, 0));
    anotherSpectral->setVelocity(Vector(1, 0));
    physics.update();
    assertEq("spectral does not receive overlap event", spectral->overlaps, 0);
    assertEq("other spectral does not receive overlap event",
             anotherSpectral->overlaps, 0);
    assertEq("spectral does not receive collision event", spectral->collisions,
             0);
    assertEq("other spectral does not receive collision event",
             anotherSpectral->collisions, 0);
    spectral->reset();
    anotherSpectral->reset();
  });

  WM.shutDown();
}

auto main() -> int {
  test("overlaps", overlaps);
  test("out of bounds", outOfBounds);
  test("kinematics", kinematics);
  suite("overlap response", overlapResponse);
  suite("events", events);
  return report();
}
