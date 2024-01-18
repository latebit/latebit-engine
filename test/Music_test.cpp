#include "../include/Music.h"

#include <sndfile.h>

#include "test.h"

auto Music_test() -> int {
  int result = 0;

  df::Music music;
  result += assert("default constructor", music.getLabel().empty());

  music.setLabel("Background Music");
  result +=
    assert("setLabel() and getLabel()", music.getLabel() == "Background Music");

  result += assert_ok("loadMusic() with valid file",
                      music.loadMusic("test/fixtures/sound.wav"));
  result += assert_fail("loadMusic() with invalid file",
                        music.loadMusic("invalid_sound.wav"));

  return result;
}