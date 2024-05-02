#include "tune.h"

#include <memory>
#include <vector>

#include "track.h"

using namespace std;

namespace sid {
Tune::Tune(int tracksCount) {
  this->bpm = 10;
  this->ticksPerBeat = 1;
  this->beatsCount = 1;
  for (int i = 0; i < tracksCount; i++) {
    this->tracks.push_back(make_shared<Track>());
  }
}

Tune::~Tune() { this->tracks.clear(); }

auto Tune::getBpm() const -> int { return this->bpm; }
void Tune::setBpm(int bpm) { this->bpm = bpm; }

auto Tune::getTicksPerBeat() const -> int { return this->ticksPerBeat; }
void Tune::setTicksPerBeat(int ticksPerBeat) {
  this->ticksPerBeat = ticksPerBeat;
}
auto Tune::getBeatsCount() const -> int { return this->beatsCount; }
auto Tune::setBeatsCount(int beatsCount) -> void {
  this->beatsCount = beatsCount;
}

auto Tune::getTracksCount() const -> int { return this->tracks.size(); }

auto Tune::getTrack(int index) const -> shared_ptr<Track> {
  return this->tracks.at(index);
}

}  // namespace sid