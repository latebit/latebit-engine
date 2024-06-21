#include "Music.h"

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include "latebit/core/audio/AudioManager.h"
#include "latebit/sid/parser/TuneParser.h"
#include "latebit/utils/Logger.h"

using namespace std;
using namespace sid;

namespace lb {
Music::Music() = default;
Music::~Music() = default;

auto Music::play(bool loop) const -> void {
  AM.playMusic(this->music.get(), loop);
}
auto Music::pause() const -> void { AM.pauseMusic(); }
auto Music::stop() const -> void { AM.stopMusic(); }

auto Music::getLabel() const -> string { return this->label; }
auto Music::setLabel(string l) -> void { this->label = l; }

auto Music::loadMusic(string filename) -> int {
  auto tune = TuneParser::fromFile(filename, &MUSIC_PARSER_OPTIONS);

  if (tune == nullptr) {
    Log.error("Music::loadMusic(): Unable to load music.");
    return -1;
  }

  this->music = std::move(tune);

  return 0;
}

}  // namespace lb
