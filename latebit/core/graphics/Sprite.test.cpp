#include "latebit/core/graphics/Sprite.h"

#include "../../../test/lib/test.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"

using namespace lb;
using namespace std;

void constructor() {
  Sprite sprite;

  // Test default values
  assertEq("width is initialized to 0", sprite.getWidth(), 0);
  assertEq("height is initialized to 0", sprite.getHeight(), 0);

  assertEq("frame count is initialized to 0", sprite.getFrameCount(), 0);
  assertEq("duration is initialized to 1", sprite.getDuration(), 1);
  assert("label is initialized to an empty string", sprite.getLabel().empty());

  Sprite sprite2("s", 1, 1, 1, {});
  Sprite sprite3(sprite2);

  assert("clones itself", sprite2 == sprite3);
}

void draw() {
  auto frames = vector<Keyframe>{Keyframe(1, 1, {Color::RED})};
  Sprite sprite("s", 1, 1, 1, frames);

  // Test draw
  assertOk("draws successfully", sprite.draw(0, Vector(0, 0)));

  // Test invalid frame number
  assertFail("fails when frame number is invalid",
             sprite.draw(10, Vector(0, 0)));
}

auto main() -> int {
  DM.startUp();
  test("constructor", constructor);
  test("draw", draw);
  test("equals (==)", []() {
    vector<Keyframe> frames = {Keyframe(1, 1, {Color::RED})};
    Sprite sprite1("s", 1, 1, 1, frames);
    Sprite sprite2("s", 1, 1, 1, frames);
    Sprite sprite3("s", 2, 1, 1, frames);
    Sprite sprite4("s", 1, 2, 1, frames);
    Sprite sprite5("s", 1, 1, 2, frames);
    Sprite sprite6("test", 1, 1, 1, frames);

    assert("equals itself", sprite1 == sprite1);
    assert("equals another sprite", sprite1 == sprite2);
    assert("not equals different width", !(sprite1 == sprite3));
    assert("not equals different height", !(sprite1 == sprite4));
    assert("not equals different duration", !(sprite1 == sprite5));
    assert("not equals different label", !(sprite1 == sprite6));
    assert("not equals empty sprite", !(sprite1 == Sprite()));
  });
  DM.shutDown();
  return report();
}