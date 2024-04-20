#pragma once

#include <memory>
#include <vector>

#include "track.h"

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
  int bpm;
  // The smallest number of notes that can be played per beat
  int ticksPerBeat;
  // How many beats in the tune
  int beatsCount;
  // The tracks in the tune
  vector<shared_ptr<Track>> tracks;

 public:
  // Creates a new tune with the given number of tracks
  Tune(int tracksCount);
  // Frees the memory associated with the tune
  ~Tune();

  // Returns the number of beats per minute
  auto getBpm() -> int;
  // Sets the number of beats per minute
  auto setBpm(int bpm) -> void;

  // Returns the number of ticks per beat
  auto getTicksPerBeat() -> int;
  // Sets the number of ticks per beat
  auto setTicksPerBeat(int ticksPerBeat) -> void;

  // Returns the number of beats in the tune
  auto getBeatsCount() -> int;
  // Sets the number of beats in the tune
  auto setBeatsCount(int beatsCount) -> void;

  // Returns the number of tracks in the tune
  auto getTracksCount() -> int;

  // Returns the track at the given index
  auto getTrack(int index) -> shared_ptr<Track>;
};
}  // namespace sid