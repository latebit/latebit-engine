#include "../include/Object.h"

#include <iostream>

#include "../include/Animation.h"
#include "../include/ResourceManager.h"
#include "../include/WorldManager.h"
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
  result += assert_vector("updates velocity", subject.getVelocity(),
                          df::Vector(1, 2));

  subject.setSpeed(2);
  subject.setDirection(df::Vector(1, 0));
  result += assert_vector("updates velocity", subject.getVelocity(),
                          df::Vector(2, 0));

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

auto Object_test() -> int {
  df::Object subject;
  int result = 0;

  // test constructor
  result += assert_int("sets an id", subject.getId(), 0);
  result += assert_string("sets a type", subject.getType(), "Object");
  result +=
      assert_vector("sets a position", subject.getPosition(), df::Vector());
  result += assert_int("sets an altitude", subject.getAltitude(),
                       df::MAX_ALTITUDE / 2);
  result +=
      assert_vector("sets a direction", subject.getDirection(), df::Vector());
  result += assert_float("sets a speed", subject.getSpeed(), 0.0);
  result += assert_int("sets a solidness", subject.getSolidness(), df::HARD);
  result += assert("sets a bounding box",
                   subject.getBox() == df::Box(df::Vector(), 1, 1));
  result +=
      assert("sets an animation", subject.getAnimation() == df::Animation());

  subject = df::Object();
  result += assert_int("increments id", subject.getId(), 1);
  subject.setId(10);
  result += assert_int("updates id", subject.getId(), 10);

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

  return result;
}