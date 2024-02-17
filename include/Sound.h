#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace lb {
class Sound {
 private:
  sf::Sound sound = sf::Sound();
  sf::SoundBuffer buffer = sf::SoundBuffer();
  std::string label = "";

 public:
  Sound();
  ~Sound();

  // Load sound buffer from a file. Return 0 if ok, else -1.
  auto loadSound(std::string filename) -> int;

  // Set label associated with sound. Used to retrieve the resource.
  auto setLabel(std::string l) -> void;
  // Get label associated with sound. Used to retrieve the resource.
  auto getLabel() const -> std::string;

  // Play sound.
  void play(bool loop = false);

  // Stop sound.
  void stop();

  // Pause sound.
  void pause();

  // Return SFML Sound
  auto getSound() const -> const sf::Sound*;
};
}  // namespace lb
