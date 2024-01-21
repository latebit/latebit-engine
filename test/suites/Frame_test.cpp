#include "Frame.h"

#include "../lib/test.h"

using namespace df;

void Frame_test() {
  test("constructor", []() {
    Frame frame1;
    assert_int("inits width", frame1.getWidth(), 0);
    assert_int("inits height", frame1.getHeight(), 0);
    assert_string("inits string", frame1.getFrameString(), "");
  });

  test("constructor with parameters", []() {
    Frame frame2(5, 3, "#####");
    assert_int("inits width", frame2.getWidth(), 5);
    assert_int("inits height", frame2.getHeight(), 3);
    assert_string("inits string", frame2.getFrameString(), "#####");
  });

  test("setters", []() {
    Frame frame3;
    frame3.setWidth(8);
    assert_int("updates width", frame3.getWidth(), 8);
    frame3.setHeight(4);
    assert_int("updates height", frame3.getHeight(), 4);
    frame3.setFrameString("########");

    assert_string("updates string", frame3.getFrameString(), "########");
  });

  test("draw", []() {
    Frame frame4(3, 2, "## #");
    assert_ok("draws the frame", frame4.draw(Vector(), Color::RED, 0));

    Frame frame5;
    assert_fail("fails on empty frame", frame5.draw(Vector(), Color::RED, 0));
  });
}