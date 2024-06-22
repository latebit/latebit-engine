#pragma once

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include <memory>
#include <string>

#include "latebit/sid/parser/TuneParser.h"
#include "latebit/sid/synth/Tune.h"

using namespace std;
using namespace sid;

namespace lb {

const ParserOptions SOUND_PARSER_OPTIONS = {
  .maxTracksCount = 2,
  .maxBeatsCount = 1,
  .maxTicksPerBeat = 16,
};

class Sound {
 public:
  // Sound is immutable
  Sound(Sound const &) = delete;
  void operator=(Sound const &) = delete;

  Sound(string label = "", unique_ptr<Tune> sound = nullptr)
    : sound(std::move(sound)), label(label){};
  ~Sound() = default;

  // Get label associated with the sound. Used to retrieve the resource.
  [[nodiscard]] auto getLabel() const -> string;

  // Play sound.
  void play(bool loop = false) const;

  // Stop sound.
  void stop() const;

  // Pause sound. It can be resumed with play().
  void pause() const;

 private:
  // Pointer to the underlying Tune resource.
  const unique_ptr<Tune> sound = nullptr;

  // Label associated with the sound. Used to retrieve the resource.
  const string label = "";
};
}  // namespace lb
