#include "../includes/Sprite.h"

#include "test.h"

bool isSameFrame(df::Frame frame1, df::Frame frame2) {
  return frame1.getWidth() == frame2.getWidth() &&
         frame1.getHeight() == frame2.getHeight() &&
         frame1.getFrameString() == frame2.getFrameString();
}

int Sprite_constructor_test() {
  printf("Sprite_constructor_test\n");
  int result = 0;

  df::Sprite sprite(5);

  // Test default values
  result += assert("width is initialized to 0", sprite.getWidth() == 0);
  result += assert("height is initialized to 0", sprite.getHeight() == 0);
  result +=
      assert("frame count is initialized to 0", sprite.getFrameCount() == 0);
  result += assert("slowdown is initialized to NO_SLOWDOWN",
                   sprite.getSlowdown() == df::NO_SLOWDOWN);
  result += assert("color is initialized to COLOR_DEFAULT",
                   sprite.getColor() == df::COLOR_DEFAULT);
  result += assert("label is initialized to an empty string",
                   sprite.getLabel().empty());

  return result;
}

int Sprite_frame_test() {
  printf("Sprite_frame_test\n");
  int result = 0;

  df::Sprite sprite(1);
  df::Frame frame1(1, 1, "a");
  df::Frame frame2(1, 2, "b");

  // Add frames
  result += assert_ok("adds successfully", sprite.addFrame(frame1));
  result += assert_int("frame count is incremented after adding a frame",
                       sprite.getFrameCount(), 1);
  result += assert("getFrame returns the correct frame",
                   isSameFrame(frame1, sprite.getFrame(0)));

  result += assert_fail("fails when maximum frame count is reached",
                        sprite.addFrame(frame2));
  result += assert_int("frame count remains the same after reaching maximum",
                       sprite.getFrameCount(), 1);

  result += assert("returns empty frame when frame number is invalid",
                   isSameFrame(df::Frame(), sprite.getFrame(10)));

  return result;
}

int Sprite_draw_test() {
  printf("Sprite_draw_test\n");
  int result = 0;

  df::Sprite sprite(1);
  df::Frame frame(1, 1, "a");

  // Add frame
  sprite.addFrame(frame);

  // Test draw
  result += assert_ok("draws successfully", sprite.draw(0, df::Vector(0, 0)));

  // Test invalid frame number
  result += assert_fail("fails when frame number is invalid",
                        sprite.draw(10, df::Vector(0, 0)));

  return result;
}

int Sprite_test() {
  int result = 0;

  df::Sprite sprite(1);
  sprite.setWidth(10);
  result += assert_int("setWidth sets width", sprite.getWidth(), 10);
  sprite.setHeight(20);
  result += assert_int("setHeight sets height", sprite.getHeight(), 20);
  sprite.setColor(df::RED);
  result += assert_int("setColor sets color", sprite.getColor(), df::RED);
  sprite.setLabel("test");
  result += assert_string("setLabel sets label", sprite.getLabel(), "test");
  sprite.setSlowdown(5);
  result += assert_int("setSlowdown sets slowdown", sprite.getSlowdown(), 5);

  result += Sprite_constructor_test();
  result += Sprite_frame_test();
  result += Sprite_draw_test();

  return result;
}