#include "Sound.h"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mixer.h>

#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {

Sound::Sound() = default;

Sound::~Sound() {
  Mix_FreeChunk(this->sound);
  this->sound = nullptr;
  this->channel = UNINITIALIZED_CHANNEL;
}

auto Sound::play(bool loop) -> void {
  this->channel = Mix_PlayChannel(this->channel, this->sound, loop ? -1 : 0);
}

auto Sound::pause() -> void { Mix_Pause(this->channel); }
auto Sound::stop() -> void {
  Mix_HaltChannel(this->channel);
  this->channel = UNINITIALIZED_CHANNEL;
}

auto Sound::getLabel() const -> string { return this->label; }
auto Sound::setLabel(string l) -> void { this->label = l; }

auto Sound::loadSound(string filename) -> int {
  if (this->sound != nullptr) {
    Mix_FreeChunk(this->sound);
    this->sound = nullptr;
  }

  this->sound = Mix_LoadWAV(filename.c_str());

  if (this->sound == nullptr) {
    Log.error("Sound::loadSound(): Unable to load sound. %s", Mix_GetError());
    return -1;
  }

  return 0;
}
}  // namespace lb