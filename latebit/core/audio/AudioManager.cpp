#include "AudioManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_timer.h>

#include <cstdint>
#include <memory>

#include "latebit/core/utils/Manager.h"
#include "latebit/sid/synth/configuration.h"
#include "latebit/sid/synth/sequencer.h"
#include "latebit/sid/synth/tune.h"
#include "latebit/utils/Logger.h"

namespace lb {
unique_ptr<sid::Sequencer> AudioManager::musicSequencer = nullptr;
unique_ptr<sid::Sequencer> AudioManager::sfxSequencer = nullptr;

auto mix(float a, float b) -> float {
  constexpr float THRESHOLD = 0.8f;
  constexpr float LIMIT_FACTOR = 0.4f;
  float mixed = a + b;

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
    float a = AudioManager::musicSequencer->getNextSample();
    float b = AudioManager::sfxSequencer->getNextSample();
    ((float *)stream)[i] = mix(a, b);
  }
}

AudioManager::AudioManager() {
  setType("AudioManager");
  AudioManager::musicSequencer = make_unique<sid::Sequencer>();
  AudioManager::sfxSequencer = make_unique<sid::Sequencer>();
  Log.debug("AudioManager::AudioManager(): Created AudioManager");
}

auto AudioManager::getInstance() -> AudioManager & {
  static AudioManager instance;
  return instance;
}

auto AudioManager::startUp() -> int {
  SDL_AudioSpec obtained,
    spec = {.freq = sid::Configuration::getSampleRate(),
            .format = AUDIO_F32,
            .channels = 1,
            .samples = (uint16_t)sid::Configuration::getBufferSize(),
            .callback = AudioManager::callback};

  this->device = SDL_OpenAudioDevice(nullptr, 0, &spec, &obtained, 1);
  if (this->device == 0) {
    Log.error("AudioManager::startUp(): Cannot open audio device. %s",
              SDL_GetError());
    return -1;
  }

  sid::Configuration::setSampleRate(obtained.freq);
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

void AudioManager::playMusic(shared_ptr<sid::Tune> tune, bool loop) {
  this->musicSequencer->setLoop(loop);
  this->musicSequencer->stop();

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

void AudioManager::playSound(shared_ptr<sid::Tune> tune, bool loop) {
  this->sfxSequencer->setLoop(loop);
  this->sfxSequencer->stop();

  // If the tune is already loaded, just play it
  if (this->sfxSequencer->getCurrentTune() != tune) {
    this->sfxSequencer->unloadTune();
    if (this->sfxSequencer->loadTune(tune) == -1) {
      Log.error("AudioManager::playSound(): Failed to load tune.");
      return;
    }
  }

  this->sfxSequencer->play();
}

void AudioManager::stopSound() { this->sfxSequencer->stop(); }
void AudioManager::pauseSound() { this->sfxSequencer->pause(); }

}  // namespace lb
