#include "core/configuration/Configuration.h"

#include "../../../test/lib/test.h"

auto main() -> int {
  test("has default values", []() -> void {
    assertEq("parses max_frame_rate", Configuration::getMaxFrameRate(), 60);

    assertEq("parses initial_window_title",
             Configuration::getInitialWindowTitle(), "LateBit");
  });

  test("parses a broken file without changing values", []() -> void {
    assertOk("does not fail",
             Configuration::fromFile("../test/fixtures/broken.cfg"));

    assertEq("parses max_frame_rate", Configuration::getMaxFrameRate(), 60);

    assertEq("parses initial_window_title",
             Configuration::getInitialWindowTitle(), "LateBit");
  });

  test("parses a correct configuration file", []() -> void {
    Configuration::fromFile("../test/fixtures/latebit.cfg");

    assertEq("parses max_frame_rate", Configuration::getMaxFrameRate(), 30);

    assertEq("parses initial_window_title",
             Configuration::getInitialWindowTitle(), "My Game");
  });

  return report();
}