#include "Music.h"

#include <SFML/Audio/Music.hpp>

#include "LogManager.h"

namespace df {

Music::Music() { this->label = ""; }

Music::~Music() { this->music.stop(); }

auto Music::play(bool loop) -> void {
  this->music.setLoop(loop);
  this->music.play();
}
auto Music::stop() -> void { this->music.stop(); }
auto Music::pause() -> void { this->music.pause(); }

auto Music::getLabel() const -> std::string { return this->label; }
auto Music::setLabel(std::string l) -> void { this->label = l; }

auto Music::loadMusic(std::string filename) -> int {
  if (!this->music.openFromFile(filename)) {
    LM.writeLog("Music::loadMusic(): Failed to load music from %s",
                filename.c_str());
    return -1;
  }

  return 0;
}

auto Music::getMusic() const -> const sf::Music* { return &this->music; }

}  // namespace df
