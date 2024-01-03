#include "../src/Object.h"

#include <iostream>

#include "../src/WorldManager.h"
#include "Object_test.h"
#include "test.h"

int altitude_test() {
  std::cout << "Object_altitude_test\n";
  int result = 0;
  df::Object subject;

  subject.setAltitude(1);
  result += assert_int("updates altitude", subject.getAltitude(), 1);

  subject.setAltitude(10);
  result +=
      assert_int("prevents out of bounds (max)", subject.getAltitude(), 1);
  subject.setAltitude(-1);
  result +=
      assert_int("prevents out of bounds (min)", subject.getAltitude(), 1);

  return result;
}

int kinematics_test() {
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

int solidness_test() {
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

int Object_test() {
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
  result += assert_float("sets a speed", subject.getSpeed(), 0);
  result += assert_int("sets a solidness", subject.getSolidness(), df::HARD);

  subject = df::Object();
  result += assert_int("increments id", subject.getId(), 1);
  subject.setId(10);
  result += assert_int("updates id", subject.getId(), 10);

  subject.setType("type");
  result += assert_string("updates type", subject.getType(), "type");

  auto position = df::Vector(1, 2);
  subject.setPosition(position);
  auto got = subject.getPosition();
  result += assert_vector("updates position", got, position);

  result += altitude_test();
  result += kinematics_test();
  result += solidness_test();

  return result;
}