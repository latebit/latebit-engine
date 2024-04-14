#include "Music.h"

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>

#include "core/utils/Logger.h"

using namespace std;

namespace lb {

Music::Music() = default;

Music::~Music() {
  Mix_FreeMusic(this->music);
  this->music = nullptr;
}

auto Music::play(bool loop) -> void {
  Mix_PlayMusic(this->music, loop ? -1 : 0);
}
auto Music::pause() -> void { Mix_PauseMusic(); }
auto Music::stop() -> void { Mix_HaltMusic(); }

auto Music::getLabel() const -> string { return this->label; }
auto Music::setLabel(string l) -> void { this->label = l; }

auto Music::loadMusic(string filename) -> int {
  if (this->music != nullptr) {
    Mix_FreeMusic(this->music);
    this->music = nullptr;
  }

  this->music = Mix_LoadMUS(filename.c_str());

  if (this->music == nullptr) {
    Log.error("Music::loadMusic(): Unable to load music. %s", Mix_GetError());
    return -1;
  }

  return 0;
}

}  // namespace lb
