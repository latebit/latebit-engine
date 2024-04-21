#pragma once

#include <SDL2/SDL_audio.h>

#include <memory>

#include "latebit/core/utils/Manager.h"
#include "latebit/sid/synth/sequencer.h"
#include "latebit/sid/synth/tune.h"

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

  void playMusic(shared_ptr<sid::Tune> tune, bool loop);
  void stopMusic();
  void pauseMusic();

  void playSound(shared_ptr<sid::Tune> tune);
  void stopSound();
  void pauseSound();

  // todo: make private
  unique_ptr<sid::Sequencer> musicSequencer;
  unique_ptr<sid::Sequencer> sfxSequencer;

 private:
  // Singleton
  AudioManager();
};
}  // namespace lb