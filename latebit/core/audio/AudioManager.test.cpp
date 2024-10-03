#include "latebit/core/audio/AudioManager.h"

#include <SDL_audio.h>

#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/sid/synth/Configuration.h"
#include "latebit/utils/Logger.h"
#include "test/lib/test.h"

auto main() -> int {
  Log.setDestination(lb::LogDestination::STDOUT);
  DM::startUp();

  test("initialization", []() {
    assertOk("starts up", AM.startUp());
    assert("configures the sample rate",
           sid::Configuration::getSampleRate() != 0);
    AM.shutDown();
  });

  DM::shutDown();
  return report();
}
