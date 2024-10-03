#include "latebit/core/graphics/Animation.h"

#include "../../../test/lib/test.h"
#include "latebit/core/GameManager.h"
#include "latebit/core/ResourceManager.h"
#include "latebit/core/geometry/Vector.h"

using namespace std;

void setSprite() {
  Animation animation;
  RM.loadTextSprite(FIXTURES_FOLDER + "/correct.lbspr", "sprite");
  auto sprite = RM.getSprite("sprite");
  animation.setIndex(1);
  animation.setSlowdownCount(1);
  animation.setSprite(sprite);

  assert("sets sprite", animation.getSprite() == sprite);
  assertEq("resets index", animation.getIndex(), 0);
  assertEq("resets duration count", animation.getSlowdownCount(), 0);
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
  assertEq("sets duration count", animation.getSlowdownCount(), 3);
}

void draw() {
  DM::startUp();
  auto filename = FIXTURES_FOLDER + "/correct.lbspr";
  auto label = "sprite";

  RM.loadTextSprite(filename, label);
  auto sprite = RM.getSprite(label);

  Animation animation;
  animation.setSprite(sprite);

  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("does't update index before duration", animation.getIndex(), 0);
  assertEq("duration count is updated", animation.getSlowdownCount(), 1);
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is updated", animation.getIndex(), 1);
  assertEq("duration is updated", animation.getSlowdownCount(), 0);

  animation.setSlowdownCount(STOP_ANIMATION_SLOWDOWN);
  assertOk("draws the frame", animation.draw(Vector()));
  assertEq("index is not updated", animation.getIndex(), 1);
  assertEq("duration is not updated", animation.getSlowdownCount(),
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
  DM::shutDown();

  static int scale = 0;
  const auto frames = vector<Keyframe>{{Color::RED}}; 
  const auto sprite2 = Sprite("s", 1, 1, 1, frames, [](Vector, const vector<Color::Color> *, uint8_t, uint8_t, uint8_t s) -> int { 
    scale = s;
    return 0;
  });
  animation.setSprite(&sprite2);
  
  assertOk("draws the frame", animation.draw(Vector(), 2));
  assertEq("uses correct scale", scale, 2);

  RM.shutDown();
  RM.startUp();
}

auto main() -> int {
  RM.startUp();

  test("setSprite", setSprite);
  test("setName", setName);
  test("setIndex", setIndex);
  test("setSlowdownCount", setSlowdownCount);
  test("draw", draw);

  RM.shutDown();
  RM.startUp();

  return report();
}