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

auto Tune::getTrack(int index) const -> const Track* {
  return this->tracks.at(index).get();
}

}  // namespace sid