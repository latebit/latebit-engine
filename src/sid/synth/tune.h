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
  // The number of tracks in the tune
  int tracksCount;
  // The tracks in the tune
  vector<shared_ptr<Track>> tracks;

 public:
  Tune(int tracksCount);
  ~Tune();
};
}  // namespace sid