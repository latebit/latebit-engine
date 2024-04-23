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

  // Loads and plays a music tune. It plays indefinitely with loop=true
  void playMusic(shared_ptr<sid::Tune> tune, bool loop);
  // Stops the music sequencer
  void stopMusic();
  // Pauses the music sequencer
  void pauseMusic();

  // Loads and play a sound effect
  void playSound(shared_ptr<sid::Tune> tune);
  // Stops the sound sequencer
  void stopSound();
  // Pauses the sound sequencer
  void pauseSound();

 private:
  // Singleton
  AudioManager();

  // The sequencer used for music
  static unique_ptr<sid::Sequencer> musicSequencer;

  // The sequencer used for sound effects
  static unique_ptr<sid::Sequencer> sfxSequencer;

  // The device used by SDL
  unsigned int device = 0;

  // The SDL audio callback used to play the sounds
  static void callback(void *userdata, Uint8 *stream, int len);
};
}  // namespace lb