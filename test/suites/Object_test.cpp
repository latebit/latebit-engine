#include "core/objects/Object.h"

#include <string>
#include <unordered_map>

#include "../lib/test.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "core/events/Event.h"
#include "core/events/EventCollision.h"
#include "core/events/EventKeyboard.h"
#include "core/events/EventOut.h"
#include "core/events/EventStep.h"
#include "core/graphics/Animation.h"
#include "core/input/InputManager.h"
#include "core/objects/ObjectList.h"
#include "core/objects/ObjectListIterator.h"
#include "core/objects/WorldManager.h"

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
  RM.loadTextSprite("test/fixtures/correct.txt", "sprite");

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

  assert_ok("subscribes to Collision", obj.subscribe(COLLISION_EVENT));
  assert_ok("subscribes to Out", obj.subscribe(OUT_EVENT));
  assert_ok("subscribes to Step", obj.subscribe(STEP_EVENT));
  assert_ok("subscribes to Keyboard", obj.subscribe(KEYBOARD_EVENT));
  assert_ok("subscribes to Mouse", obj.subscribe(MSE_EVENT));
  assert_ok("subscribes to custom", obj.subscribe("custom"));

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

  Event customEvent;
  customEvent.setType("custom");
  WM.onEvent(&customEvent);
  assert_int("responds to custom event",
             Object_eventSubscription_test_emittedCount["custom"], 1);

  assert_ok("unsubscribes to Collision", obj.unsubscribe(COLLISION_EVENT));
  assert_ok("unsubscribes to Out", obj.unsubscribe(OUT_EVENT));
  assert_ok("unsubscribes to Step", obj.unsubscribe(STEP_EVENT));
  assert_ok("unsubscribes to Keyboard", obj.unsubscribe(KEYBOARD_EVENT));
  assert_ok("unsubscribes to Mouse", obj.unsubscribe(MSE_EVENT));
  assert_ok("unsubscribes to custom", obj.unsubscribe("custom"));

  WM.onEvent(&collision);
  assert_int("does not respond to Collision",
             Object_eventSubscription_test_emittedCount[COLLISION_EVENT], 1);

  WM.onEvent(&out);
  assert_int("does not respond to Out",
             Object_eventSubscription_test_emittedCount[OUT_EVENT], 1);

  GM.onEvent(&step);
  assert_int("does not respond to Step",
             Object_eventSubscription_test_emittedCount[STEP_EVENT], 1);

  IM.onEvent(&keyboard);
  assert_int("does not respond to Keyboard",
             Object_eventSubscription_test_emittedCount[KEYBOARD_EVENT], 1);

  IM.onEvent(&mouse);
  assert_int("does not respond to Mouse",
             Object_eventSubscription_test_emittedCount[MSE_EVENT], 1);

  WM.onEvent(&customEvent);
  assert_int("does not respond to custom event",
             Object_eventSubscription_test_emittedCount["custom"], 1);
}

void Object_visible_test() {
  Object subject;

  subject.setVisible(false);
  assert("is invisible", !subject.isVisible());
  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    assert("does not appear in visible list for altitude " + to_string(i),
           WM.getSceneGraph().getVisibleObjects(i).find(&subject) == -1);
  }

  subject.setVisible(true);
  assert("is visible", subject.isVisible());
  assert(
    "appears in visible list for altitude " + to_string(subject.getAltitude()),
    WM.getSceneGraph().getVisibleObjects(subject.getAltitude()).find(&subject) >
      -1);

  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    if (i == subject.getAltitude()) continue;

    assert("does not appear in visible for altitude " + to_string(i),
           WM.getSceneGraph().getVisibleObjects(i).find(&subject) == -1);
  }
}

void Object_active_test() {
  Object subject;

  subject.setActive(false);
  assert("sets active to false", !subject.isActive());
  assert_int("does not appear in active objects",
             WM.getSceneGraph().getActiveObjects().find(&subject), -1);
  assert("appears in inactive objects",
         WM.getSceneGraph().getInactiveObjects().find(&subject) > -1);

  subject.setActive(true);
  assert("sets active to true", subject.isActive());
  assert("does not appear in inactive objects",
         WM.getSceneGraph().getInactiveObjects().find(&subject) == -1);
  assert("appears in active objects",
         WM.getSceneGraph().getActiveObjects().find(&subject) > -1);
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

    RM.loadTextSprite("test/fixtures/correct.txt", "sprite");
    assert_ok("sets valid sprite", subject.setSprite("sprite"));
    assert_fail("does not set invalid sprite", subject.setSprite("invalid"));

    auto position = Vector(1, 2);
    subject.setPosition(position);
    auto got = subject.getPosition();
    assert_vector("updates position", got, position);

    auto box = Box(Vector(), 1, 1);
    subject.setBox(box);
    assert("updates the box", subject.getBox() == box);

    subject.setDebug(true);
    assert("updates debug", subject.getDebug());
  });

  test("altitude", Object_altitude_test);
  test("kinematics", Object_kinematics_test);
  test("solidness", Object_solidness_test);
  test("boundingBox", Object_boundingBox_test);
  test("eventSubscription", Object_eventSubscription_test);
  test("visible", Object_visible_test);
  test("active", Object_active_test);
}