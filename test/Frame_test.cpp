#include "../include/Frame.h"

#include "test.h"

auto Frame_test() -> int {
  int result = 0;

  printf("Frame_emptyConstructor_test\n");
  df::Frame frame1;
  result += assert_int("inits width", frame1.getWidth(), 0);
  result += assert_int("inits height", frame1.getHeight(), 0);
  result += assert_string("inits string", frame1.getFrameString(), "");

  printf("Frame_parametrisedConstructor_test\n");
  df::Frame frame2(5, 3, "#####");
  result += assert_int("inits width", frame2.getWidth(), 5);
  result += assert_int("inits height", frame2.getHeight(), 3);
  result += assert_string("inits string", frame2.getFrameString(), "#####");

  df::Frame frame3;
  frame3.setWidth(8);
  result += assert_int("updates width", frame3.getWidth(), 8);
  frame3.setHeight(4);
  result += assert_int("updates height", frame3.getHeight(), 4);
  frame3.setFrameString("########");
  result +=
    assert_string("updates string", frame3.getFrameString(), "########");

  printf("Frame_draw_test\n");
  df::Frame frame4(3, 2, "###");
  result +=
    assert_ok("draws the frame", frame4.draw(df::Vector(), df::Color::RED, 0));

  df::Frame frame5;
  result += assert_fail("fails on empty frame",
                        frame5.draw(df::Vector(), df::Color::RED, 0));

  return result;
}