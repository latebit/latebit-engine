#include "core/audio/Sound.h"

#include "../lib/test.h"
#include "core/audio/AudioManager.h"

void Sound_test() {
  AM.startUp();
  test("constructor", []() {
    Sound sound;
    assert("initializes the label", sound.getLabel().empty());
  });

  test("setters", []() {
    Sound sound;
    sound.setLabel("Background Music");
    assert_string("updates the label", sound.getLabel(), "Background Music");
  });

  test("loadSound", []() {
    Sound sound;
    assert_ok("loads valid file", sound.loadSound("test/fixtures/sound.wav"));
    assert_fail("doesn't load invalid file",
                sound.loadSound("invalid_sound.wav"));
  });
  AM.shutDown();
  AM.startUp();
}