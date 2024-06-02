#include "latebit/core/graphics/Animation.h"

#include "../../../test/lib/test.h"
#include "latebit/core/GameManager.h"
#include "latebit/core/ResourceManager.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/graphics/Sprite.h"

using namespace std;

void setSprite() {
  Animation animation;
  RM.loadTextSprite(FIXTURES_FOLDER + "/correct.txt", "sprite");
  Sprite *sprite = RM.getSprite("sprite");
  animation.setIndex(1);
  animation.setSlowdownCount(1);
  animation.setSprite(sprite);

  assert("sets sprite", animation.getSprite() == sprite);
  assertEq("resets index", animation.getIndex(), 0);
  assertEq("resets slowdown count", animation.getSlowdownCount(), 0);
}

void setName() {
  Animation animation;
  animation.setName("animation");
  assertEq("sets name", animation.getName(), "animation");
}

void setIndex() {
  Animation animation;
  animation.setIndex(5);
  assertEq("sets index", animation.getIndex(), 5);
}

void setSlowdownCount() {
  Animation animation;
  animation.setSlowdownCount(3);
  assertEq("sets slowdown count", animation.getSlowdownCount(), 3);
}

void draw() {
  auto filename = FIXTURES_FOLDER + "/correct.txt";
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

  animation.setSlowdownCount(0);
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is updated", animation.getIndex(), 1);
  GM.pause();
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is not updated", animation.getIndex(), 1);
  GM.resume();
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is updated", animation.getIndex(), 0);

  RM.shutDown();
  RM.startUp();
}

void getBox() {
  auto animation = Animation();
  auto unitBox = Box(Vector(), 1.0, 1.0);
  assertEq("returns unit box", animation.getBox(), unitBox);

  auto filename = FIXTURES_FOLDER + "/correct.txt";
  auto label = "label";
  RM.loadTextSprite(filename, label);
  Sprite *sprite = RM.getSprite(label);
  animation.setSprite(sprite);

  auto want = Box(Vector(), 3.0, 4.0);
  auto animBox = animation.getBox();
  assertEq("returns correct box", animBox, want);
}

auto main() -> int {
  DM.startUp();
  RM.startUp();

  test("setSprite", setSprite);
  test("setName", setName);
  test("setIndex", setIndex);
  test("setSlowdownCount", setSlowdownCount);
  test("draw", draw);
  test("getBox", getBox);

  RM.shutDown();
  RM.startUp();
  DM.shutDown();
  DM.startUp();

  return report();
}