#include "core/audio/Music.h"

#include "../../../test/lib/test.h"
#include "core/audio/AudioManager.h"

auto main() -> int {
  AM.startUp();

  test("constructor", []() {
    Music music;
    assert("sets empty label", music.getLabel().empty());
  });

  test("setters", []() {
    Music music;
    music.setLabel("Background Music");
    assert("setLabel() and getLabel()", music.getLabel() == "Background Music");
  });

  test("loadMusic", []() {
    Music music;
    assertOk("succeeds with valid file",
             music.loadMusic(FIXTURES_FOLDER + "/sound.wav"));
    assertFail("fails with invalid file", music.loadMusic("invalid_sound.wav"));
  });

  AM.shutDown();
  return report();
}