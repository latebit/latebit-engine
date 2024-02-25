#pragma once

#include <SDL2/SDL_audio.h>

#include <string>

#include "Manager.h"
#include "Music.h"

using namespace std;

#define AM lb::AudioManager::getInstance()

namespace lb {
class AudioManager : public Manager {
 public:
  static auto getInstance() -> AudioManager &;
  AudioManager(AudioManager const &) = delete;
  void operator=(AudioManager const &) = delete;

  // Choses the default audio device and gets it ready for playback.
  auto startUp() -> int override;

  // Frees the audio device chosen in startUp.
  void shutDown() override;

 private:
  // Singleton
  AudioManager();
};
}  // namespace lb