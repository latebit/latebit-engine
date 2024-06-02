#include "AudioManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_timer.h>

#include <cstdint>
#include <memory>

#include "latebit/core/utils/Manager.h"
#include "latebit/sid/synth/Configuration.h"
#include "latebit/sid/synth/Sequencer.h"
#include "latebit/sid/synth/Tune.h"
#include "latebit/utils/Logger.h"

using namespace std;
using namespace sid;

namespace lb {
unique_ptr<Sequencer> AudioManager::musicSequencer = nullptr;
array<unique_ptr<Sequencer>, 4> AudioManager::sfxSequencers = {
  nullptr, nullptr, nullptr, nullptr};

auto softLimit(float mixed) -> float {
  constexpr float THRESHOLD = 0.8f;
  constexpr float LIMIT_FACTOR = 0.4f;

  // Simple soft limiting
  if (mixed > THRESHOLD)
    mixed = THRESHOLD + (mixed - THRESHOLD) * LIMIT_FACTOR;
  else if (mixed < -THRESHOLD)
    mixed = -THRESHOLD + (mixed + THRESHOLD) * LIMIT_FACTOR;

  return mixed;
}

void AudioManager::callback([[maybe_unused]] void *userdata, Uint8 *stream,
                            int len) {
  size_t samples = (size_t)len / sizeof(float);

  for (size_t i = 0; i < samples; i++) {
    float mixed = AudioManager::musicSequencer->getNextSample();
    for (auto &sequencer : AudioManager::sfxSequencers) {
      if (sequencer != nullptr) mixed += sequencer->getNextSample();
    }

    ((float *)stream)[i] = softLimit(mixed);
  }
}

AudioManager::AudioManager() {
  setType("AudioManager");
  AudioManager::musicSequencer = make_unique<Sequencer>();
  for (auto &sequencer : AudioManager::sfxSequencers)
    sequencer = make_unique<Sequencer>();

  Log.debug("AudioManager::AudioManager(): Created AudioManager");
}

auto AudioManager::getInstance() -> AudioManager & {
  static AudioManager instance;
  return instance;
}

auto AudioManager::startUp() -> int {
  SDL_AudioSpec obtained,
    spec = {.freq = Configuration::getSampleRate(),
            .format = AUDIO_F32,
            .channels = 1,
            .samples = (uint16_t)Configuration::getBufferSize(),
            .callback = AudioManager::callback};

  this->device = SDL_OpenAudioDevice(nullptr, 0, &spec, &obtained, 1);
  if (this->device == 0) {
    Log.error("AudioManager::startUp(): Cannot open audio device. %s",
              SDL_GetError());
    return -1;
  }

  Configuration::setSampleRate(obtained.freq);
  SDL_PauseAudioDevice(this->device, 0);

  Log.info("AudioManager::startUp(): Started successfully");
  return Manager::startUp();
}

auto AudioManager::shutDown() -> void {
  SDL_CloseAudioDevice(this->device);
  this->device = 0;
  Manager::shutDown();
  Log.info("AudioManager::shutDown(): Shut down successfully");
}

void AudioManager::playMusic(Tune *tune, bool loop) {
  this->musicSequencer->setLoop(loop);

  // If the tune is already loaded, just play it
  if (this->musicSequencer->getCurrentTune() != tune) {
    this->musicSequencer->unloadTune();
    if (this->musicSequencer->loadTune(tune) == -1) {
      Log.error("AudioManager::playMusic(): Failed to load tune.");
      return;
    }
  }

  this->musicSequencer->play();
}

void AudioManager::stopMusic() { this->musicSequencer->stop(); }

void AudioManager::pauseMusic() { this->musicSequencer->pause(); }

void AudioManager::playSound(Tune *tune, bool loop) {
  Sequencer *sequencer = nullptr;
  // Find a sequencer that is not playing or is playing the same tune
  for (auto &s : this->sfxSequencers) {
    if (!s->isPlaying() || s->getCurrentTune() == tune) {
      sequencer = s.get();
      break;
    }
  }

  if (sequencer == nullptr) {
    Log.error(
      "AudioManager::playSound(): Cannot play sound. All sfxSequencers are "
      "already busy.");
    return;
  }

  sequencer->setLoop(loop);

  // If the tune is already loaded, just play it
  if (sequencer->getCurrentTune() != tune) {
    sequencer->unloadTune();
    if (sequencer->loadTune(tune) == -1) {
      Log.error("AudioManager::playSound(): Failed to load tune.");
      return;
    }
  }

  sequencer->play();
}

void AudioManager::stopSound(Tune *tune) {
  Sequencer *sequencer = nullptr;
  // Find a sequencer that is playing the same tune
  for (auto &s : this->sfxSequencers) {
    if (s->getCurrentTune() == tune) {
      sequencer = s.get();
      break;
    }
  }

  if (sequencer == nullptr) {
    Log.error(
      "AudioManager::stopSound(): Cannot stop sound. Unable to find sequencer "
      "for given tune");
    return;
  }

  sequencer->stop();
}

void AudioManager::pauseSound(Tune *tune) {
  Sequencer *sequencer = nullptr;
  // Find a sequencer that is playing the same tune
  for (auto &s : this->sfxSequencers) {
    if (s->getCurrentTune() == tune) {
      sequencer = s.get();
      break;
    }
  }

  if (sequencer == nullptr) {
    Log.error(
      "AudioManager::pauseSound(): Cannot pause sound. Unable to find "
      "sequencer for given tune");
    return;
  }

  sequencer->pause();
}

}  // namespace lb
