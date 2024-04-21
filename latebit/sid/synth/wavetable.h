#pragma once

#include <array>
#include <cstdlib>
#include <vector>

#include "latebit/sid/synth/oscillator.h"

using namespace std;

namespace sid {
class WaveTable {
  friend class Configuration;

 private:
  // A table containing the values of a precomputed waveform for each wave type
  // type.
  static array<vector<float>, 4> WAVE_TABLE;

  // Populates the wave table with the values of the waveforms in size quants.
  static void createWaveTable(int size);

 public:
  // Returns the value of the waveform for the given wave type and sample.
  static auto get(WaveType waveType, float step) -> float;

  // Returns the size of the wave table.
  static auto getSize() -> int;
};
};  // namespace sid