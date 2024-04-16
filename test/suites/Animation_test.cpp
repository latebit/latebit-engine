#include "core/graphics/Animation.h"

#include "../lib/test.h"
#include "core/ResourceManager.h"
#include "core/geometry/Box.h"
#include "core/geometry/Vector.h"
#include "core/graphics/DisplayManager.h"
#include "core/graphics/Sprite.h"

using namespace std;

void setSprite_test() {
  Animation animation;
  RM.loadTextSprite("test/fixtures/correct.txt", "sprite");
  Sprite *sprite = RM.getSprite("sprite");
  animation.setIndex(1);
  animation.setSlowdownCount(1);
  animation.setSprite(sprite);

  assert("sets sprite", animation.getSprite() == sprite);
  assertEq("resets index", animation.getIndex(), 0);
  assertEq("resets slowdown count", animation.getSlowdownCount(), 0);
}

void setName_test() {
  Animation animation;
  animation.setName("animation");
  assertEq("sets name", animation.getName(), "animation");
}

void setIndex_test() {
  Animation animation;
  animation.setIndex(5);
  assertEq("sets index", animation.getIndex(), 5);
}

void setSlowdownCount_test() {
  Animation animation;
  animation.setSlowdownCount(3);
  assertEq("sets slowdown count", animation.getSlowdownCount(), 3);
}

void draw_test() {
  auto filename = "test/fixtures/correct.txt";
  auto label = "sprite";

  RM.loadTextSprite(filename, label);
  Sprite *sprite = RM.getSprite(label);

  Animation animation;
  animation.setSprite(sprite);

  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("does't update index before slowdown", animation.getIndex(), 0);
  assertEq("slowdown count is updated", animation.getSlowdownCount(), 1);
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is updated", animation.getIndex(), 1);
  assertEq("slowdown is updated", animation.getSlowdownCount(), 0);

  // Test animation slowdown
  animation.setSlowdownCount(STOP_ANIMATION_SLOWDOWN);
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is not updated", animation.getIndex(), 1);
  assertEq("slowdown is not updated", animation.getSlowdownCount(),
           STOP_ANIMATION_SLOWDOWN);

  RM.shutDown();
  RM.startUp();
}

void getBox_test() {
  auto animation = Animation();
  auto unitBox = Box(Vector(), 1.0, 1.0);
  assertEq("returns unit box", animation.getBox(), unitBox);

  auto filename = "test/fixtures/correct.txt";
  auto label = "label";
  RM.loadTextSprite(filename, label);
  Sprite *sprite = RM.getSprite(label);
  animation.setSprite(sprite);

  auto want = Box(Vector(), 3.0, 4.0);
  auto animBox = animation.getBox();
  assertEq("returns correct box", animBox, want);
}

void Animation_test() {
  DM.startUp();
  RM.startUp();
  test("setSprite", setSprite_test);
  test("setName", setName_test);
  test("setIndex", setIndex_test);
  test("setSlowdownCount", setSlowdownCount_test);
  test("draw", draw_test);
  test("getBox", getBox_test);
  RM.shutDown();
  RM.startUp();
  DM.shutDown();
  DM.startUp();
}