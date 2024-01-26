#include "Object.h"

#include <unordered_map>

#include "../lib/test.h"
#include "Animation.h"
#include "EventCollision.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventOut.h"
#include "EventStep.h"
#include "GameManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

void Object_altitude_test() {
  Object subject;

  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    subject.setAltitude(i);
    assert_int("updates altitude", subject.getAltitude(), i);
  }

  subject.setAltitude(1);
  assert("updates current scene",
         WM.getSceneGraph().getVisibleObjects(1).find(&subject) > -1);

  auto initialAltitude = subject.getAltitude();

  subject.setAltitude(10);
  assert_int("prevents out of bounds (max)", subject.getAltitude(),
             initialAltitude);
  subject.setAltitude(-1);
  assert_int("prevents out of bounds (min)", subject.getAltitude(),
             initialAltitude);
}

void Object_kinematics_test() {
  Object subject;

  subject.setSpeed(1);
  assert_float("updates speed", subject.getSpeed(), 1);

  subject.setDirection(Vector(1, 1));
  assert_vector("updates direction", subject.getDirection(), Vector(1, 1));

  subject.setVelocity(Vector(1, 2));

  assert_vector("updates velocity", subject.getVelocity(), Vector(1, 2));

  subject.setSpeed(2);
  subject.setDirection(Vector(1, 0));

  assert_vector("updates velocity", subject.getVelocity(), Vector(2, 0));
}

void Object_solidness_test() {
  Object subject;

  subject.setSolidness(SOFT);
  assert_int("updates solidness", subject.getSolidness(), SOFT);
  assert("updates current scene",
         WM.getSceneGraph().getSolidObjects().find(&subject) > -1);

  assert("SOFT is solid", subject.isSolid());
  subject.setSolidness(HARD);
  assert("HARD is solid", subject.isSolid());
  subject.setSolidness(SPECTRAL);
  assert("SPECTRAL is not solid", !subject.isSolid());
}

void Object_boundingBox_test() {
  Object subject;
  subject.setPosition(Vector(1, 1));
  RM.loadSprite("test/fixtures/correct.txt", "sprite");

  subject.setSprite("sprite");

  assert_box("sets bounding box", subject.getBox(), Box(Vector(), 3, 4));
  assert_box("gets bounding box in world coordinates", subject.getWorldBox(),
             Box(Vector(1, 1), 3, 4));
  assert_box("gets bounding box in world coordinates relative to (2, 2)",
             subject.getWorldBox(Vector(2, 2)), Box(Vector(2, 2), 3, 4));
}

unordered_map<string, int> Object_eventSubscription_test_emittedCount = {};
void Object_eventSubscription_test() {
  Object subject;

  class TestObject : public Object {
   public:
    TestObject() { this->setType("TestObject"); };
    auto eventHandler(const Event* e) -> int override {
      Object_eventSubscription_test_emittedCount[e->getType()]++;
      return 0;
    };
  };

  TestObject obj;

  assert_ok("subscribes to Collision", WM.subscribe(&obj, COLLISION_EVENT));
  assert_ok("subscribes to Out", WM.subscribe(&obj, OUT_EVENT));
  assert_ok("subscribes to Step", GM.subscribe(&obj, STEP_EVENT));

  assert_ok("subscribes to Keyboard", IM.subscribe(&obj, KEYBOARD_EVENT));
  assert_ok("subscribes to Mouse", IM.subscribe(&obj, MSE_EVENT));

  EventCollision collision;
  WM.onEvent(&collision);
  assert_int("responds to Collision",
             Object_eventSubscription_test_emittedCount[COLLISION_EVENT], 1);
  EventOut out;
  WM.onEvent(&out);
  assert_int("responds to Out",
             Object_eventSubscription_test_emittedCount[OUT_EVENT], 1);

  EventStep step;
  GM.onEvent(&step);
  assert_int("responds to Step",
             Object_eventSubscription_test_emittedCount[STEP_EVENT], 1);

  EventKeyboard keyboard;
  IM.onEvent(&keyboard);
  assert_int("responds to Keyboard",
             Object_eventSubscription_test_emittedCount[KEYBOARD_EVENT], 1);

  EventMouse mouse;
  IM.onEvent(&mouse);
  assert_int("responds to Mouse",
             Object_eventSubscription_test_emittedCount[MSE_EVENT], 1);
}

void Object_test() {
  test("constructor", []() {
    Object subject;
    // test constructor
    assert_string("sets a type", subject.getType(), "Object");

    assert_vector("sets a position", subject.getPosition(), Vector());
    assert_int("sets an altitude", subject.getAltitude(), 0);

    assert_vector("sets a direction", subject.getDirection(), Vector());
    assert_float("sets a speed", subject.getSpeed(), 0.0);
    assert_int("sets a solidness", subject.getSolidness(), HARD);
    assert("sets a bounding box", subject.getBox() == Box(Vector(), 1, 1));

    assert("sets an animation", subject.getAnimation() == Animation());
  });

  test("ids", []() {
    Object subject;
    int lastId = subject.getId();
    subject = Object();
    assert_int("increments id", subject.getId(), lastId + 1);
  });

  test("setters", []() {
    Object subject;
    subject.setType("type");
    assert_string("updates type", subject.getType(), "type");

    auto animation = Animation();
    animation.setName("sprite");
    subject.setAnimation(animation);
    assert("updates animation", subject.getAnimation() == animation);

    RM.loadSprite("test/fixtures/correct.txt", "sprite");
    assert_ok("sets valid sprite", subject.setSprite("sprite"));
    assert_fail("does not set invalid sprite", subject.setSprite("invalid"));

    auto position = Vector(1, 2);
    subject.setPosition(position);
    auto got = subject.getPosition();
    assert_vector("updates position", got, position);

    auto box = Box(Vector(), 1, 1);
    subject.setBox(box);
    assert("updates the box", subject.getBox() == box);
  });

  test("altitude", Object_altitude_test);
  test("kinematics", Object_kinematics_test);
  test("solidness", Object_solidness_test);
  test("boundingBox", Object_boundingBox_test);
  test("eventSubscription", Object_eventSubscription_test);
}