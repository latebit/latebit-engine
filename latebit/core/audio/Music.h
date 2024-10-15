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
  Music(string label = "", unique_ptr<Tune> music = nullptr)
    : music(std::move(music)), label(label) {};
  ~Music() = default;

  // Get label associated with the music. Used to retrieve the resource.
  [[nodiscard]] auto getLabel() const -> string;

  // Play music.
  void play(bool loop = false) const;

  // Stop music.
  void stop() const;

  // Pause music. It can be resumed with play().
  void pause() const;

 private:
  // Pointer to the underlying sid::Tune resource.
  const unique_ptr<Tune> music = nullptr;

  // Label associated with the music. Used to retrieve the resource.
  const string label = "";
};
}  // namespace lb
