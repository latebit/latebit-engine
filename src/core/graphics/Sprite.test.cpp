#include "core/graphics/Sprite.h"

#include "../../../test/lib/test.h"
#include "core/graphics/Colors.h"
#include "core/graphics/DisplayManager.h"

using namespace lb;
using namespace std;

void constructor() {
  Sprite sprite;

  // Test default values
  assertEq("width is initialized to 0", sprite.getWidth(), 0);
  assertEq("height is initialized to 0", sprite.getHeight(), 0);

  assertEq("frame count is initialized to 0", sprite.getFrameCount(), 0);
  assert("slowdown is initialized to NO_SLOWDOWN",
         sprite.getSlowdown() == NO_SLOWDOWN);
  assert("label is initialized to an empty string", sprite.getLabel().empty());

  Sprite sprite2("s", 1, 1, 1, 1);
  Sprite sprite3(sprite2);

  assert("clones itself", sprite2 == sprite3);
}

void frame() {
  Sprite sprite("s", 1, 1, 1, 1);
  Frame frame1(1, 1, {Color::RED});
  Frame frame2(1, 1, {Color::BLUE});
  Frame frame3(2, 2, {Color::BLUE});

  // Add frames
  assertOk("adds successfully", sprite.addFrame(frame1));
  assertEq("frame count is incremented after adding a frame",
           sprite.getFrameCount(), 1);
  assert("getFrame returns the correct frame", frame1 == sprite.getFrame(0));

  assertFail("fails when maximum frame count is reached",
             sprite.addFrame(frame2));
  assertEq("frame count remains the same after reaching maximum",
           sprite.getFrameCount(), 1);

  assertFail("fails when frame has incorrect size", sprite.addFrame(frame3));

  assert("returns empty frame when frame number is invalid",
         Frame() == sprite.getFrame(10));
}

void draw() {
  Sprite sprite("s", 1, 1, 1, 1);
  Frame frame(1, 1, {Color::RED});

  // Add frame
  sprite.addFrame(frame);

  // Test draw
  assertOk("draws successfully", sprite.draw(0, Vector(0, 0)));

  // Test invalid frame number
  assertFail("fails when frame number is invalid",
             sprite.draw(10, Vector(0, 0)));
}

auto main() -> int {
  DM.startUp();
  test("setters", []() {
    Sprite sprite("s", 1, 1, 1, 1);
    sprite.setWidth(10);
    assertEq("setWidth sets width", sprite.getWidth(), 10);
    sprite.setHeight(20);
    assertEq("setHeight sets height", sprite.getHeight(), 20);
    sprite.setLabel("test");
    assertEq("setLabel sets label", sprite.getLabel(), "test");
    sprite.setSlowdown(5);
    assertEq("setSlowdown sets slowdown", sprite.getSlowdown(), 5);
  });

  test("constructor", constructor);
  test("frame", frame);
  test("draw", draw);
  test("equals (==)", []() {
    Sprite sprite1("s", 1, 1, 1, 1);
    Sprite sprite2("s", 1, 1, 1, 1);
    Sprite sprite3("s", 2, 1, 1, 1);
    Sprite sprite4("s", 1, 2, 1, 1);
    Sprite sprite5("s", 1, 1, 2, 1);
    Sprite sprite6("test", 1, 1, 1, 1);

    assert("equals itself", sprite1 == sprite1);
    assert("equals another sprite", sprite1 == sprite2);
    assert("not equals different width", !(sprite1 == sprite3));
    assert("not equals different height", !(sprite1 == sprite4));
    assert("not equals different slowdown", !(sprite1 == sprite5));
    assert("not equals different label", !(sprite1 == sprite6));
    assert("not equals empty sprite", !(sprite1 == Sprite()));
  });
  DM.shutDown();
  return report();
}