#pragma once

#include <memory>
#include <vector>

#include "Note.h"

using namespace std;

namespace sid {
// The tune is the main structure that represents a song/composition/sound
// effect. It contains the tempo, the number of beats, the number of tracks and
// the tracks themselves.
//
// See the docs/spec-v0.md for more information.
class Tune {
 private:
  // The number of beats per minute
  const int bpm;
  // The smallest number of notes that can be played per beat
  const int ticksPerBeat;
  // How many beats in the tune
  const int beatsCount;
  // The tracks in the tune
  const vector<unique_ptr<Track>> tracks;

 public:
  // Creates a new tune with the given number of tracks
  Tune(int bpm, int ticksPerBeat, int beatsCount,
       vector<unique_ptr<Track>>&& tracks)
    : bpm(bpm),
      ticksPerBeat(ticksPerBeat),
      beatsCount(beatsCount),
      tracks(std::move(tracks)) {}

  // Frees the memory associated with the tune
  ~Tune() = default;

  // Returns the number of beats per minute
  [[nodiscard]] auto getBpm() const -> int;

  // Returns the number of ticks per beat
  [[nodiscard]] auto getTicksPerBeat() const -> int;

  // Returns the number of beats in the tune
  [[nodiscard]] auto getBeatsCount() const -> int;

  // Returns the number of tracks in the tune
  [[nodiscard]] auto getTracksCount() const -> int;

  // Returns the track at the given index
  [[nodiscard]] auto getTrack(int index) const -> const Track*;
};
}  // namespace sid