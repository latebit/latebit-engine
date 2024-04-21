#include "Music.h"

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include "latebit/core/audio/AudioManager.h"
#include "latebit/sid/parser/parser.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {

Music::Music() = default;

Music::~Music() {
  this->music.reset();
  this->music = nullptr;
}

auto Music::play(bool loop) -> void { AM.playMusic(this->music, loop); }
auto Music::pause() -> void { AM.pauseMusic(); }
auto Music::stop() -> void { AM.stopMusic(); }

auto Music::getLabel() const -> string { return this->label; }
auto Music::setLabel(string l) -> void { this->label = l; }

auto Music::loadMusic(string filename) -> int {
  if (this->music != nullptr) {
    this->music.reset();
    this->music = nullptr;
  }

  this->music = sid::TuneParser::fromFile(filename);

  if (this->music == nullptr) {
    Log.error("Music::loadMusic(): Unable to load music. %s");
    return -1;
  }

  return 0;
}

}  // namespace lb
