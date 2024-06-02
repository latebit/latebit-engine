#include "latebit/core/audio/Sound.h"

#include "../../../test/lib/test.h"
#include "latebit/core/audio/AudioManager.h"

auto main() -> int {
  AM.startUp();
  test("constructor", []() {
    Sound sound;
    assert("initializes the label", sound.getLabel().empty());
  });

  test("setters", []() {
    Sound sound;
    sound.setLabel("Background Music");
    assertEq("updates the label", sound.getLabel(), "Background Music");
  });

  test("loadSound", []() {
    Sound sound;
    assertOk("loads valid file",
             sound.loadSound(FIXTURES_FOLDER + "/sfx.lbsfx"));
    assertFail("doesn't load invalid file", sound.loadSound("sound.wav"));
  });
  AM.shutDown();
  return report();
}