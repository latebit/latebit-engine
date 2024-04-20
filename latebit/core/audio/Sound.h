#pragma once

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>

#include <string>

using namespace std;

namespace lb {

const int UNINITIALIZED_CHANNEL = -1;

class Sound {
 public:
  Sound();
  ~Sound();

  // Load sound buffer from a file. Return 0 if ok, else -1.
  auto loadSound(string filename) -> int;

  // Set label associated with sound. Used to retrieve the resource.
  auto setLabel(string l) -> void;
  // Get label associated with sound. Used to retrieve the resource.
  [[nodiscard]] auto getLabel() const -> string;

  // Play sound.
  void play(bool loop = false);

  // Stop sound.
  void stop();

  // Pause sound.
  void pause();

 private:
  // Pointer to the chunk resource in SDL_mixer representation.
  Mix_Chunk* sound = nullptr;

  // Channel on which the current sound will be playing.
  // It will be chosen upon the first `play` call.
  int channel = UNINITIALIZED_CHANNEL;

  // Label associated with sound. Used to retrieve the resource.
  string label = "";
};
}  // namespace lb
