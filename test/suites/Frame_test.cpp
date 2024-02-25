#include "Frame.h"

#include <vector>

#include "../lib/test.h"
#include "DisplayManager.h"

using namespace lb;

void Frame_test() {
  DM.startUp();

  test("constructor", []() {
    Frame frame;
    assert_int("inits width", frame.getWidth(), 0);
    assert_int("inits height", frame.getHeight(), 0);
    assert("inits content", frame.getContent().empty());
  });

  test("constructor with parameters", []() {
    vector<Color> content = {Color::RED, Color::GREEN, Color::BLUE};
    Frame frame(5, 3, content);
    assert_int("inits width", frame.getWidth(), 5);
    assert_int("inits height", frame.getHeight(), 3);
    assert("inits content", frame.getContent() == content);
  });

  test("draw", []() {
    vector<Color> content = {Color::RED, Color::GREEN, Color::BLUE};
    Frame frame(3, 2, content);
    assert_ok("draws the frame", frame.draw(Vector()));

    Frame empty;
    assert_fail("fails on empty frame", empty.draw(Vector()));
  });

  test("equals (==)", []() {
    vector<Color> content = {Color::RED, Color::GREEN, Color::BLUE};
    vector<Color> otherContent = {Color::RED, Color::GREEN, Color::RED};
    Frame frame1(3, 2, content);
    Frame frame2(3, 2, content);
    Frame frame3(3, 2, otherContent);
    assert("equals itself", frame1 == frame1);
    assert("equals another frame", frame1 == frame2);
    assert("not equals different frame", !(frame1 == frame3));
    assert("not equals empty frame", !(frame1 == Frame()));
  });

  DM.shutDown();
  DM.startUp();
}