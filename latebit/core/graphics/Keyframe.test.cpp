#include "latebit/core/graphics/Keyframe.h"

#include <vector>

#include "latebit/core/graphics/DisplayManager.h"
#include "test/lib/test.h"

using namespace lb;

auto main() -> int {
  DM.startUp();

  test("constructor", []() {
    Keyframe frame;
    assertEq("inits width", frame.getWidth(), 0);
    assertEq("inits height", frame.getHeight(), 0);
    assert("inits content", frame.getContent().empty());
  });

  test("constructor with parameters", []() {
    vector<Color> content = {Color::RED, Color::GREEN, Color::BLUE};
    Keyframe frame(5, 3, content);
    assertEq("inits width", frame.getWidth(), 5);
    assertEq("inits height", frame.getHeight(), 3);
    assert("inits content", frame.getContent() == content);
  });

  test("draw", []() {
    vector<Color> content = {Color::RED, Color::GREEN, Color::BLUE};
    Keyframe frame(3, 1, content);
    assertOk("draws the frame", frame.draw(Vector()));

    Keyframe empty;
    assertOk("doesn't fail on empty frame", empty.draw(Vector()));
  });

  test("equals (==)", []() {
    vector<Color> content = {Color::RED, Color::GREEN, Color::BLUE};
    vector<Color> otherContent = {Color::RED, Color::GREEN, Color::RED};
    Keyframe frame1(3, 1, content);
    Keyframe frame2(3, 1, content);
    Keyframe frame3(3, 1, otherContent);
    assert("equals itself", frame1 == frame1);
    assert("equals another frame", frame1 == frame2);
    assert("not equals different frame", !(frame1 == frame3));
    assert("not equals empty frame", !(frame1 == Keyframe()));
  });

  DM.shutDown();
  DM.startUp();

  return report();
}