#include "../include/Sound.h"

#include <sndfile.h>

#include "test.h"

auto Sound_test() -> int {
  int result = 0;

  df::Sound sound;
  result += assert("default constructor", sound.getLabel().empty());

  sound.setLabel("Background Music");
  result +=
    assert("setLabel() and getLabel()", sound.getLabel() == "Background Music");

  result += assert_ok("loadSound() with valid file",
                      sound.loadSound("test/fixtures/sound.wav"));
  result += assert_fail("loadSound() with invalid file",
                        sound.loadSound("invalid_sound.wav"));

  return result;
}