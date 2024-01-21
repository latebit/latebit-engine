#include "Sprite.h"

#include "../lib/test.h"

using namespace df;

auto isSameFrame(Frame frame1, Frame frame2) -> bool {
  return frame1.getWidth() == frame2.getWidth() &&
         frame1.getHeight() == frame2.getHeight() &&
         frame1.getFrameString() == frame2.getFrameString();
}

void Sprite_constructor_test() {
  Sprite sprite(5);

  // Test default values
  assert_int("width is initialized to 0", sprite.getWidth(), 0);
  assert_int("height is initialized to 0", sprite.getHeight(), 0);

  assert_int("frame count is initialized to 0", sprite.getFrameCount(), 0);
  assert("slowdown is initialized to NO_SLOWDOWN",
         sprite.getSlowdown() == NO_SLOWDOWN);
  assert("color is initialized to COLOR_DEFAULT",
         sprite.getColor() == COLOR_DEFAULT);
  assert("label is initialized to an empty string", sprite.getLabel().empty());
}

void Sprite_frame_test() {
  Sprite sprite(1);
  Frame frame1(1, 1, "a");
  Frame frame2(1, 2, "b");

  // Add frames
  assert_ok("adds successfully", sprite.addFrame(frame1));
  assert_int("frame count is incremented after adding a frame",
             sprite.getFrameCount(), 1);
  assert("getFrame returns the correct frame",
         isSameFrame(frame1, sprite.getFrame(0)));

  assert_fail("fails when maximum frame count is reached",
              sprite.addFrame(frame2));
  assert_int("frame count remains the same after reaching maximum",
             sprite.getFrameCount(), 1);

  assert("returns empty frame when frame number is invalid",
         isSameFrame(Frame(), sprite.getFrame(10)));
}

void Sprite_draw_test() {
  Sprite sprite(1);
  Frame frame(1, 1, "a");

  // Add frame
  sprite.addFrame(frame);

  // Test draw
  assert_ok("draws successfully", sprite.draw(0, Vector(0, 0)));

  // Test invalid frame number
  assert_fail("fails when frame number is invalid",
              sprite.draw(10, Vector(0, 0)));
}

void Sprite_test() {
  test("setters", []() {
    Sprite sprite(1);
    sprite.setWidth(10);
    assert_int("setWidth sets width", sprite.getWidth(), 10);
    sprite.setHeight(20);
    assert_int("setHeight sets height", sprite.getHeight(), 20);
    sprite.setColor(RED);
    assert_int("setColor sets color", sprite.getColor(), RED);
    sprite.setLabel("test");
    assert_string("setLabel sets label", sprite.getLabel(), "test");
    sprite.setSlowdown(5);
    assert_int("setSlowdown sets slowdown", sprite.getSlowdown(), 5);
  });

  test("constructor", Sprite_constructor_test);
  test("frame", Sprite_frame_test);
  test("draw", Sprite_draw_test);
}