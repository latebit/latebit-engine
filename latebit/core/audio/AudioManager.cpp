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
void callback(void *userdata, Uint8 *stream, int len) {
  int samples = len / sizeof(float);

  for (int i = 0; i < samples; i++) {
    float a = AM.musicSequencer->getNextSample();
    float b = AM.sfxSequencer->getNextSample();
    ((float *)stream)[i] = (a == 0.0f) ? b : (b == 0.0f ? a : (a + b) / 2.0f);
  }
}

AudioManager::AudioManager() {
  setType("AudioManager");
  Log.debug("AudioManager::AudioManager(): Created AudioManager");
}

auto AudioManager::getInstance() -> AudioManager & {
  static AudioManager instance;
  return instance;
}

auto AudioManager::startUp() -> int {
  this->musicSequencer = make_unique<sid::Sequencer>();
  this->sfxSequencer = make_unique<sid::Sequencer>();

  SDL_AudioSpec obtained,
    spec = {.freq = sid::Configuration::getSampleRate(),
            .format = AUDIO_F32,
            .channels = 1,
            .samples = (uint16_t)sid::Configuration::getBufferSize(),
            .callback = callback};

  unsigned int d = SDL_OpenAudioDevice(nullptr, 0, &spec, &obtained, 1);
  sid::Configuration::setSampleRate(obtained.freq);
  SDL_PauseAudioDevice(d, 0);

  Log.info("AudioManager::startUp(): Started successfully");
  return Manager::startUp();
}

auto AudioManager::shutDown() -> void {
  Manager::shutDown();
  Log.info("AudioManager::shutDown(): Shut down successfully");
}

void AudioManager::playMusic(shared_ptr<sid::Tune> tune, bool loop) {
  this->musicSequencer->setLoop(loop);
  this->musicSequencer->stop();
  this->musicSequencer->unloadTune();
  this->musicSequencer->loadTune(tune);
  this->musicSequencer->play();
}

void AudioManager::stopMusic() { this->musicSequencer->stop(); }
void AudioManager::pauseMusic() { this->musicSequencer->pause(); }

void AudioManager::playSound(shared_ptr<sid::Tune> tune) {
  this->sfxSequencer->unloadTune();
  this->sfxSequencer->loadTune(tune);
  this->sfxSequencer->play();
}

void AudioManager::stopSound() { this->sfxSequencer->stop(); }
void AudioManager::pauseSound() { this->sfxSequencer->pause(); }

}  // namespace lb