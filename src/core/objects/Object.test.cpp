#include "core/objects/Object.h"

#include <string>
#include <unordered_map>

#include "../../../test/lib/test.h"
#include "core/GameManager.h"
#include "core/ResourceManager.h"
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

void altitude() {
  Object subject;

  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    subject.setAltitude(i);
    assertEq("updates altitude", subject.getAltitude(), i);
  }

  subject.setAltitude(1);
  assert("updates current scene",
         WM.getSceneGraph().getVisibleObjects(1).find(&subject) > -1);

  auto initialAltitude = subject.getAltitude();

  subject.setAltitude(10);
  assertEq("prevents out of bounds (max)", subject.getAltitude(),
           initialAltitude);
  subject.setAltitude(-1);
  assertEq("prevents out of bounds (min)", subject.getAltitude(),
           initialAltitude);
}

void kinematics() {
  Object subject;

  subject.setSpeed(1);
  assertEq("updates speed", subject.getSpeed(), 1.0);

  subject.setDirection(Vector(1, 1));
  assertEq("updates direction", subject.getDirection(), Vector(1, 1));

  subject.setVelocity(Vector(1, 2));

  assertEq("updates velocity", subject.getVelocity(), Vector(1, 2));

  subject.setSpeed(2);
  subject.setDirection(Vector(1, 0));

  assertEq("updates velocity", subject.getVelocity(), Vector(2, 0));
}

void solidness() {
  Object subject;

  subject.setSolidness(SOFT);
  assertEq("updates solidness", subject.getSolidness(), SOFT);
  assert("updates current scene",
         WM.getSceneGraph().getSolidObjects().find(&subject) > -1);

  assert("SOFT is solid", subject.isSolid());
  subject.setSolidness(HARD);
  assert("HARD is solid", subject.isSolid());
  subject.setSolidness(SPECTRAL);
  assert("SPECTRAL is not solid", !subject.isSolid());
}

void boundingBox() {
  Object subject;
  subject.setPosition(Vector(1, 1));
  RM.loadTextSprite(FIXTURES_FOLDER + "/correct.txt", "sprite");

  subject.setSprite("sprite");

  assertEq("sets bounding box", subject.getBox(), Box(Vector(), 3, 4));
  assertEq("gets bounding box in world coordinates", subject.getWorldBox(),
           Box(Vector(1, 1), 3, 4));
  assertEq("gets bounding box in world coordinates relative to (2, 2)",
           subject.getWorldBox(Vector(2, 2)), Box(Vector(2, 2), 3, 4));
  RM.unloadSprite("sprite");
}

unordered_map<string, int> Object_eventSubscription_test_emittedCount = {};
void eventSubscription() {
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

  assertOk("subscribes to Collision", obj.subscribe(COLLISION_EVENT));
  assertOk("subscribes to Out", obj.subscribe(OUT_EVENT));
  assertOk("subscribes to Step", obj.subscribe(STEP_EVENT));
  assertOk("subscribes to Keyboard", obj.subscribe(KEYBOARD_EVENT));
  assertOk("subscribes to Mouse", obj.subscribe(MSE_EVENT));
  assertOk("subscribes to custom", obj.subscribe("custom"));

  EventCollision collision;
  WM.onEvent(&collision);
  assertEq("responds to Collision",
           Object_eventSubscription_test_emittedCount[COLLISION_EVENT], 1);
  EventOut out;
  WM.onEvent(&out);
  assertEq("responds to Out",
           Object_eventSubscription_test_emittedCount[OUT_EVENT], 1);

  EventStep step;
  GM.onEvent(&step);
  assertEq("responds to Step",
           Object_eventSubscription_test_emittedCount[STEP_EVENT], 1);

  EventKeyboard keyboard;
  IM.onEvent(&keyboard);
  assertEq("responds to Keyboard",
           Object_eventSubscription_test_emittedCount[KEYBOARD_EVENT], 1);

  EventMouse mouse;
  IM.onEvent(&mouse);
  assertEq("responds to Mouse",
           Object_eventSubscription_test_emittedCount[MSE_EVENT], 1);

  Event customEvent;
  customEvent.setType("custom");
  WM.onEvent(&customEvent);
  assertEq("responds to custom event",
           Object_eventSubscription_test_emittedCount["custom"], 1);

  assertOk("unsubscribes to Collision", obj.unsubscribe(COLLISION_EVENT));
  assertOk("unsubscribes to Out", obj.unsubscribe(OUT_EVENT));
  assertOk("unsubscribes to Step", obj.unsubscribe(STEP_EVENT));
  assertOk("unsubscribes to Keyboard", obj.unsubscribe(KEYBOARD_EVENT));
  assertOk("unsubscribes to Mouse", obj.unsubscribe(MSE_EVENT));
  assertOk("unsubscribes to custom", obj.unsubscribe("custom"));

  WM.onEvent(&collision);
  assertEq("does not respond to Collision",
           Object_eventSubscription_test_emittedCount[COLLISION_EVENT], 1);

  WM.onEvent(&out);
  assertEq("does not respond to Out",
           Object_eventSubscription_test_emittedCount[OUT_EVENT], 1);

  GM.onEvent(&step);
  assertEq("does not respond to Step",
           Object_eventSubscription_test_emittedCount[STEP_EVENT], 1);

  IM.onEvent(&keyboard);
  assertEq("does not respond to Keyboard",
           Object_eventSubscription_test_emittedCount[KEYBOARD_EVENT], 1);

  IM.onEvent(&mouse);
  assertEq("does not respond to Mouse",
           Object_eventSubscription_test_emittedCount[MSE_EVENT], 1);

  WM.onEvent(&customEvent);
  assertEq("does not respond to custom event",
           Object_eventSubscription_test_emittedCount["custom"], 1);
}

void visible() {
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

void active() {
  Object subject;

  subject.setActive(false);
  assert("sets active to false", !subject.isActive());
  assertEq("does not appear in active objects",
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

auto main() -> int {
  test("constructor", []() {
    Object subject;
    // test constructor
    assertEq("sets a type", subject.getType(), "Object");

    assertEq("sets a position", subject.getPosition(), Vector());
    assertEq("sets an altitude", subject.getAltitude(), 0);

    assertEq("sets a direction", subject.getDirection(), Vector());
    assertEq("sets a speed", subject.getSpeed(), 0.0);
    assertEq("sets a solidness", subject.getSolidness(), HARD);
    assert("sets a bounding box", subject.getBox() == Box(Vector(), 1, 1));

    assert("sets an animation", subject.getAnimation() == Animation());
  });

  test("ids", []() {
    Object subject;
    int lastId = subject.getId();
    subject = Object();
    assertEq("increments id", subject.getId(), lastId + 1);
  });

  test("setters", []() {
    Object subject;
    subject.setType("type");
    assertEq("updates type", subject.getType(), "type");

    auto animation = Animation();
    animation.setName("sprite");
    subject.setAnimation(animation);
    assert("updates animation", subject.getAnimation() == animation);

    RM.loadTextSprite(FIXTURES_FOLDER + "/correct.txt", "sprite");
    assertOk("sets valid sprite", subject.setSprite("sprite"));
    assertFail("does not set invalid sprite", subject.setSprite("invalid"));
    RM.unloadSprite("sprite");

    auto position = Vector(1, 2);
    subject.setPosition(position);
    auto got = subject.getPosition();
    assertEq("updates position", got, position);

    auto box = Box(Vector(), 1, 1);
    subject.setBox(box);
    assert("updates the box", subject.getBox() == box);

    subject.setDebug(true);
    assert("updates debug", subject.getDebug());
  });

  test("altitude", altitude);
  test("kinematics", kinematics);
  test("solidness", solidness);
  test("boundingBox", boundingBox);
  test("eventSubscription", eventSubscription);
  test("visible", visible);
  test("active", active);

  return report();
}