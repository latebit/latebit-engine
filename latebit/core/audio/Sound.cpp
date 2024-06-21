#include "Sound.h"

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include "latebit/core/audio/AudioManager.h"
#include "latebit/sid/parser/TuneParser.h"
#include "latebit/utils/Logger.h"

using namespace std;
using namespace sid;

namespace lb {
Sound::Sound() = default;
Sound::~Sound() = default;

auto Sound::play(bool loop) const -> void {
  AM.playSound(this->sound.get(), loop);
}
auto Sound::pause() const -> void { AM.pauseSound(this->sound.get()); }
auto Sound::stop() const -> void { AM.stopSound(this->sound.get()); }

auto Sound::getLabel() const -> string { return this->label; }
auto Sound::setLabel(string l) -> void { this->label = l; }

auto Sound::loadSound(string filename) -> int {
  auto tune = TuneParser::fromFile(filename, &SOUND_PARSER_OPTIONS);

  if (tune == nullptr) {
    Log.error("Sound::loadSound(): Unable to load sound.");
    return -1;
  }

  this->sound = std::move(tune);

  return 0;
}

}  // namespace lb
