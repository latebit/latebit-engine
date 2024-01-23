#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace df {
class Music {
 private:
  // Music is immutable
  Music(Music const &) = delete;
  // Music is immutable
  void operator=(Music const &) = delete;

  sf::Music music = sf::Music();
  std::string label = "";

 public:
  Music();
  ~Music();

  // Load music from a file. Return 0 if ok, else -1.
  auto loadMusic(std::string filename) -> int;

  // Set label associated with sound. Used to retrieve the resource.
  auto setLabel(std::string label) -> void;
  // Get label associated with sound. Used to retrieve the resource.
  auto getLabel() const -> std::string;

  // Play sound.
  void play(bool loop = false);

  // Stop sound.
  void stop();

  // Pause sound.
  void pause();

  // Return SFML Music
  auto getMusic() const -> const sf::Music *;
};
}  // namespace df
