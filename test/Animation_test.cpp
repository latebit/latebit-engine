#include "../include/Animation.h"

#include "../include/Box.h"
#include "../include/ResourceManager.h"
#include "../include/Sprite.h"
#include "../include/Vector.h"
#include "../include/utils.h"
#include "Animation_test.h"
#include "SpriteParser_test.h"
#include "test.h"

int setSprite_test() {
  printf("setSprite_test\n");

  df::Animation animation;
  makeFile("sprite", "1\n3\n4\n2\nblue\n***\n***\n***\n***");
  RM.loadSprite("sprite", "sprite");
  df::Sprite *sprite = RM.getSprite("sprite");
  animation.setSprite(sprite);

  int result = assert("sets sprite", animation.getSprite() == sprite);

  remove("sprite");
  RM.shutDown();
  RM.startUp();

  return result;
}

int setName_test() {
  printf("setName_test\n");
  df::Animation animation;
  animation.setName("animation");
  return assert_string("sets name", animation.getName(), "animation");
}

int setIndex_test() {
  printf("setIndex_test\n");
  df::Animation animation;
  animation.setIndex(5);
  return assert_int("sets index", animation.getIndex(), 5);
}

int setSlowdownCount_test() {
  printf("setSlowdownCount_test\n");
  df::Animation animation;
  animation.setSlowdownCount(3);
  return assert_int("sets slowdown count", animation.getSlowdownCount(), 3);
}

int draw_test() {
  printf("draw_test\n");

  auto filename = "sprite";
  auto label = "sprite";

  makeFile(filename,
           "2\n3\n4\n2\nblue\n***\n***\n***\n***\n@@@\n@@@\n@@@\n@@@");
  RM.loadSprite(filename, label);
  df::Sprite *sprite = RM.getSprite(label);

  df::Animation animation;
  animation.setSprite(sprite);
  animation.setIndex(0);
  animation.setSlowdownCount(0);

  int result = assert_ok("draws the frame", animation.draw(df::Vector()));
  result += assert_int("does't update index before slowdown",
                       animation.getIndex(), 0);
  result +=
      assert_int("slowdown count is updated", animation.getSlowdownCount(), 1);

  result += assert_ok("draws the frame", animation.draw(df::Vector()));
  result += assert_int("index is updated", animation.getIndex(), 1);
  result += assert_int("slowdown is updated", animation.getSlowdownCount(), 0);

  // Test animation slowdown
  animation.setSlowdownCount(df::STOP_ANIMATION_SLOWDOWN);
  result += assert_ok("draws the frame", animation.draw(df::Vector()));
  result += assert_int("index is not updated", animation.getIndex(), 1);
  result += assert_int("slowdown is not updated", animation.getSlowdownCount(),
                       df::STOP_ANIMATION_SLOWDOWN);

  remove(filename);
  RM.shutDown();
  RM.startUp();
  return result;
}

int getBox_test() {
  int result = 0;
  printf("getBox_test\n");

  auto animation = df::Animation();

  result += assert("returns unit box",
                   animation.getBox() == df::Box(df::Vector(-0.5, -0.5), 1, 1));

  auto filename = "sprite";
  auto label = "label";
  makeFile(filename,
           "2\n3\n4\n2\nblue\n***\n***\n***\n***\n@@@\n@@@\n@@@\n@@@");
  RM.loadSprite(filename, label);
  df::Sprite *sprite = RM.getSprite(label);
  animation.setSprite(sprite);

  auto want = df::Box(df::Vector(-1.5, -2.0), 3.0, 4.0);
  auto animBox = animation.getBox();
  result += assert("returns correct box", animBox == want);

  remove(filename);
  return result;
}

int Animation_test() {
  int result = 0;
  result += setSprite_test();
  result += setName_test();
  result += setIndex_test();
  result += setSlowdownCount_test();
  result += draw_test();
  result += getBox_test();
  return result;
}