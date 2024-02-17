#include "Frame.h"

#include <vector>

#include "../lib/test.h"

using namespace lb;

void Frame_test() {
  test("constructor", []() {
    Frame frame;
    assert_int("inits width", frame.getWidth(), 0);
    assert_int("inits height", frame.getHeight(), 0);
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
}