#pragma once

#include <SDL2/SDL_audio.h>

#include <memory>

#include "latebit/core/utils/Manager.h"
#include "latebit/sid/synth/Sequencer.h"
#include "latebit/sid/synth/Tune.h"

using namespace std;

#define AM lb::AudioManager::getInstance()

namespace lb {
// The AudioManager class is a singleton that manages the audio devices and
// sequencers used to play music and sound effects.
// Latebit allows to play one music track and up to four sound effects at the
// same time. This is achieved by using a single audio device and a callback
// function that mixes the samples from the music and sound sequencers.
// The only supported sounds are those in the SID format.
// You can read more about the SID format
// [here](https://github.com/latebit/latebit-engine/blob/master/docs/specs/sid-v0.md)
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

  // Loads and play a sound effect. It plays indefinitely with loop=true
  void playSound(shared_ptr<sid::Tune> tune, bool loop);
  // Stops the sound sequencer with the given tune
  void stopSound(shared_ptr<sid::Tune> tune);
  // Pauses the sound sequencer with the given tune
  void pauseSound(shared_ptr<sid::Tune> tune);

 private:
  // Singleton
  AudioManager();

  // The sequencer used for music
  static unique_ptr<sid::Sequencer> musicSequencer;

  // The sequencers used for sound effects
  static array<unique_ptr<sid::Sequencer>, 4> sfxSequencers;

  // The device used by SDL
  unsigned int device = 0;

  // The SDL audio callback used to play the sounds
  static void callback(void *userdata, Uint8 *stream, int len);
};
}  // namespace lb