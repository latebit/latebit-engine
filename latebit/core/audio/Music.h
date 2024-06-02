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

const ParserOptions MUSIC_PARSER_OPTIONS = {
  .maxTracksCount = 3,
  .maxBeatsCount = 64,
  .maxTicksPerBeat = 16,
};

class Music {
 public:
  // Music is immutable
  Music(Music const &) = delete;
  void operator=(Music const &) = delete;

  Music();
  ~Music();

  // Load music from a file. Return 0 if ok, else -1.
  auto loadMusic(string filename) -> int;

  // Set label associated with the music. Used to retrieve the resource.
  auto setLabel(string label) -> void;

  // Get label associated with the music. Used to retrieve the resource.
  [[nodiscard]] auto getLabel() const -> string;

  // Play music.
  void play(bool loop = false);

  // Stop music.
  void stop();

  // Pause music. It can be resumed with play().
  void pause();

 private:
  // Pointer to the underlying sid::Tune resource.
  unique_ptr<Tune> music = nullptr;

  // Label associated with the music. Used to retrieve the resource.
  string label = "";
};
}  // namespace lb
