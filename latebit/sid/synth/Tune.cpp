#include "Tune.h"

#include <memory>
#include <vector>

#include "Note.h"

using namespace std;

namespace sid {
auto Tune::getBpm() const -> int { return this->bpm; }
auto Tune::getTicksPerBeat() const -> int { return this->ticksPerBeat; }
auto Tune::getBeatsCount() const -> int { return this->beatsCount; }
auto Tune::getTracksCount() const -> int { return this->tracks.size(); }

auto Tune::getTrack(int index) const -> const unique_ptr<Track>& {
  return this->tracks.at(index);
}

auto Tune::operator==(const Tune& other) const -> bool {
  bool result = beatsCount == other.beatsCount && bpm == other.bpm &&
                ticksPerBeat == other.ticksPerBeat &&
                tracks.size() == other.tracks.size();

  if (!result) return result;

  for (int i = 0; i < getTracksCount(); i++) {
    auto& track = this->getTrack(i);
    auto& otherTrack = other.getTrack(i);

    if (track->size() != otherTrack->size()) {
      return false;
    }

    for (size_t j = 0; j < track->size(); j++) {
      if (track->at(j) != otherTrack->at(j)) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace sid