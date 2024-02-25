#pragma once

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>

#include <cstddef>
#include <string>

using namespace std;

namespace lb {
class Music {
 public:
  // Music is immutable
  Music(Music const &) = delete;
  void operator=(Music const &) = delete;

  Music();
  ~Music();

  // Load music from a file. Return 0 if ok, else -1.
  auto loadMusic(string filename) -> int;

  // Set label associated with sound. Used to retrieve the resource.
  auto setLabel(string label) -> void;
  // Get label associated with sound. Used to retrieve the resource.
  [[nodiscard]] auto getLabel() const -> string;

  // Play music.
  void play(bool loop = false);
  // Pause music.
  void pause();
  // Stop music.
  void stop();

 private:
  // Pointer to the music resource in SDL_mixer representation.
  Mix_Music *music = nullptr;
  // Label associated with sound. Used to retrieve the resource.
  string label = "";
};
}  // namespace lb
