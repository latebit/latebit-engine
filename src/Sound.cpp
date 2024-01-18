#include "Sound.h"

#include "LogManager.h"

namespace df {

Sound::Sound() = default;

Sound::~Sound() {
  this->sound.stop();
  this->sound.resetBuffer();
}

auto Sound::play(bool loop) -> void {
  this->sound.setLoop(loop);
  this->sound.play();
}

auto Sound::stop() -> void { this->sound.stop(); }

auto Sound::pause() -> void { this->sound.pause(); }

auto Sound::getLabel() const -> std::string { return this->label; }
auto Sound::setLabel(std::string l) -> void { this->label = l; }

auto Sound::loadSound(std::string filename) -> int {
  if (!this->buffer.loadFromFile(filename)) {
    LM.writeLog("Sound::loadSound(): Failed to load sound from %s",
                filename.c_str());
    return -1;
  }
  this->sound.setBuffer(this->buffer);
  return 0;
}

auto Sound::getSound() const -> const sf::Sound* { return &this->sound; }
}  // namespace df