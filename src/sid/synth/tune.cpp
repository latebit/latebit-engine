#include "tune.h"

#include "track.h"

using namespace std;

namespace sid {
Tune::Tune(int tracksCount) {
  this->bpm = 10;
  this->ticksPerBeat = 1;
  this->beatsCount = 1;
  this->tracks = vector<Track>(tracksCount);
}

Tune::~Tune() { this->tracks.clear(); }

auto Tune::getBpm() -> int { return this->bpm; }
void Tune::setBpm(int bpm) { this->bpm = bpm; }

auto Tune::getTicksPerBeat() -> int { return this->ticksPerBeat; }
void Tune::setTicksPerBeat(int ticksPerBeat) {
  this->ticksPerBeat = ticksPerBeat;
}
auto Tune::getBeatsCount() -> int { return this->beatsCount; }
auto Tune::setBeatsCount(int beatsCount) -> void {
  this->beatsCount = beatsCount;
}

auto Tune::getTracksCount() -> int { return this->tracks.capacity(); }

auto Tune::getTrack(int index) -> Track { return this->tracks[index]; }
void Tune::setTrack(int index, Track track) { this->tracks[index] = track; }

}  // namespace sid