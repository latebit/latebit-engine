#include "Configuration.h"

#include "../lib/test.h"

void Configuration_test() {
  test("has default values", []() -> void {
    assert_int("parses max_frame_rate", Configuration::getMaxFrameRate(), 60);

    assert_string("parses initial_window_title",
                  Configuration::getInitialWindowTitle(), "LateBit");
  });

  test("parses a broken file without changing values", []() -> void {
    assert_ok("does not fail",
              Configuration::fromFile("test/fixtures/broken.cfg"));

    assert_int("parses max_frame_rate", Configuration::getMaxFrameRate(), 60);

    assert_string("parses initial_window_title",
                  Configuration::getInitialWindowTitle(), "LateBit");
  });

  test("parses a correct configuration file", []() -> void {
    Configuration::fromFile("test/fixtures/latebit.cfg");

    assert_int("parses max_frame_rate", Configuration::getMaxFrameRate(), 30);

    assert_string("parses initial_window_title",
                  Configuration::getInitialWindowTitle(), "My Game");
  });

  Configuration::reset();
}