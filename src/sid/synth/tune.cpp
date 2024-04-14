#include "tune.h"

#include <memory>

#include "track.h"

using namespace std;

namespace sid {
Tune::Tune(int tracksCount) {
  this->bpm = 10;
  this->ticksPerBeat = 1;
  this->beatsCount = 1;
  this->tracksCount = tracksCount;
  this->tracks = vector<shared_ptr<Track>>(tracksCount);
}

Tune::~Tune() { this->tracks.clear(); }

}  // namespace sid