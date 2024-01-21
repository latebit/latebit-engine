#include "../include/Object.h"

#include <iostream>
#include <unordered_map>

#include "../include/Animation.h"
#include "../include/ResourceManager.h"
#include "../include/WorldManager.h"
#include "EventCollision.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventOut.h"
#include "EventStep.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Object_test.h"
#include "SpriteParser_test.h"
#include "test.h"

auto Object_altitude_test() -> int {
  std::cout << "Object_altitude_test\n";
  int result = 0;
  df::Object subject;

  for (int i = 0; i <= df::MAX_ALTITUDE; i++) {
    subject.setAltitude(i);
    result += assert_int("updates altitude", subject.getAltitude(), i);
  }

  auto initialAltitude = subject.getAltitude();

  subject.setAltitude(10);
  result += assert_int("prevents out of bounds (max)", subject.getAltitude(),
                       initialAltitude);
  subject.setAltitude(-1);
  result += assert_int("prevents out of bounds (min)", subject.getAltitude(),
                       initialAltitude);

  return result;
}

auto Object_kinematics_test() -> int {
  std::cout << "Object_kinematics_test\n";
  df::Object subject;
  int result = 0;

  subject.setSpeed(1);
  result += assert_float("updates speed", subject.getSpeed(), 1);

  subject.setDirection(df::Vector(1, 1));
  result += assert_vector("updates direction", subject.getDirection(),
                          df::Vector(1, 1));

  subject.setVelocity(df::Vector(1, 2));
  result +=
    assert_vector("updates velocity", subject.getVelocity(), df::Vector(1, 2));

  subject.setSpeed(2);
  subject.setDirection(df::Vector(1, 0));
  result +=
    assert_vector("updates velocity", subject.getVelocity(), df::Vector(2, 0));

  return result;
}

auto Object_solidness_test() -> int {
  std::cout << "Object_solidness_test\n";
  df::Object subject;
  int result = 0;

  subject.setSolidness(df::SOFT);
  result += assert_int("updates solidness", subject.getSolidness(), df::SOFT);

  result += assert("SOFT is solid", subject.isSolid());
  subject.setSolidness(df::HARD);
  result += assert("HARD is solid", subject.isSolid());
  subject.setSolidness(df::SPECTRAL);
  result += assert("SPECTRAL is not solid", !subject.isSolid());

  return result;
}

auto Object_boundingBox_test() -> int {
  int result = 0;
  printf("Object_boundingBox_test\n");

  df::Object subject;
  subject.setPosition(df::Vector(1, 1));
  makeFile("sprite.txt", "1\n3\n4\n2\nblue\n***\n***\n***\n***");
  RM.loadSprite("sprite.txt", "sprite");
  remove("sprite.txt");

  subject.setSprite("sprite");

  result += assert("sets bounding box",
                   subject.getBox() == df::Box(df::Vector(), 3, 4));

  result += assert("gets bounding box in world coordinates",
                   subject.getWorldBox() == df::Box(df::Vector(1, 1), 3, 4));

  result += assert(
    "gets bounding box in world coordinates relative to (2, 2)",
    subject.getWorldBox(df::Vector(2, 2)) == df::Box(df::Vector(2, 2), 3, 4));

  return result;
}

unordered_map<string, int> Object_eventSubscription_test_emittedCount = {};
auto Object_eventSubscription_test() -> int {
  int result = 0;
  printf("Object_eventSubscription_test\n");

  df::Object subject;

  class TestObject : public df::Object {
   public:
    TestObject() { this->setType("TestObject"); };
    auto eventHandler(const df::Event* e) -> int override {
      Object_eventSubscription_test_emittedCount[e->getType()]++;
      return 0;
    };
  };

  TestObject obj;

  result +=
    assert_ok("subscribes to Collision", WM.subscribe(&obj, COLLISION_EVENT));
  result += assert_ok("subscribes to Out", WM.subscribe(&obj, OUT_EVENT));
  result += assert_ok("subscribes to Step", GM.subscribe(&obj, STEP_EVENT));
  result +=
    assert_ok("subscribes to Keyboard", IM.subscribe(&obj, KEYBOARD_EVENT));
  result += assert_ok("subscribes to Mouse", IM.subscribe(&obj, MSE_EVENT));

  WM.onEvent(new EventCollision());
  result +=
    assert_int("responds to Collision",
               Object_eventSubscription_test_emittedCount[COLLISION_EVENT], 1);

  WM.onEvent(new EventOut());
  result +=
    assert_int("responds to Out",
               Object_eventSubscription_test_emittedCount[OUT_EVENT], 1);

  GM.onEvent(new EventStep());
  result +=
    assert_int("responds to Step",
               Object_eventSubscription_test_emittedCount[STEP_EVENT], 1);

  IM.onEvent(new EventKeyboard());
  result +=
    assert_int("responds to Keyboard",
               Object_eventSubscription_test_emittedCount[KEYBOARD_EVENT], 1);

  IM.onEvent(new EventMouse());
  result +=
    assert_int("responds to Mouse",
               Object_eventSubscription_test_emittedCount[MSE_EVENT], 1);

  return result;
}

auto Object_test() -> int {
  df::Object subject;
  int result = 0;

  // test constructor
  result += assert_string("sets a type", subject.getType(), "Object");
  result +=
    assert_vector("sets a position", subject.getPosition(), df::Vector());
  result += assert_int("sets an altitude", subject.getAltitude(), 0);
  result +=
    assert_vector("sets a direction", subject.getDirection(), df::Vector());
  result += assert_float("sets a speed", subject.getSpeed(), 0.0);
  result += assert_int("sets a solidness", subject.getSolidness(), df::HARD);
  result += assert("sets a bounding box",
                   subject.getBox() == df::Box(df::Vector(), 1, 1));
  result +=
    assert("sets an animation", subject.getAnimation() == df::Animation());

  int lastId = subject.getId();
  subject = df::Object();
  result += assert_int("increments id", subject.getId(), lastId + 1);

  subject.setType("type");
  result += assert_string("updates type", subject.getType(), "type");

  subject.setAltitude(1);
  result += assert_int("updates altitude", subject.getAltitude(), 1);

  auto animation = df::Animation();
  animation.setName("sprite");
  subject.setAnimation(animation);
  result += assert("updates animation", subject.getAnimation() == animation);

  makeFile("sprite.txt", "1\n3\n4\n2\nblue\n***\n***\n***\n***");
  RM.loadSprite("sprite.txt", "sprite");
  remove("sprite.txt");

  result += assert_ok("sets valid sprite", subject.setSprite("sprite"));
  result +=
    assert_fail("does not set invalid sprite", subject.setSprite("invalid"));

  auto position = df::Vector(1, 2);
  subject.setPosition(position);
  auto got = subject.getPosition();
  result += assert_vector("updates position", got, position);

  auto box = df::Box(df::Vector(), 1, 1);
  subject.setBox(box);
  result += assert("updates the box", subject.getBox() == box);

  result += Object_altitude_test();
  result += Object_kinematics_test();
  result += Object_solidness_test();
  result += Object_boundingBox_test();
  result += Object_eventSubscription_test();

  return result;
}