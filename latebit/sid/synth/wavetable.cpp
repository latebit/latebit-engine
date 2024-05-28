#include "wavetable.h"

#include <array>
#include <cstdlib>

using namespace std;

namespace sid {

array<vector<float>, 4> WaveTable::WAVE_TABLE = {
  vector<float>(), vector<float>(), vector<float>(), vector<float>()};

void WaveTable::createWaveTable(int size) {
  for (int i = 0; i < 4; i++) {
    WAVE_TABLE[i] = {};
    WAVE_TABLE[i].reserve(size);
  }

  for (int i = 0; i < size; i++) {
    float x = (float)i / size;
    if (x < 0.25f) {
      WAVE_TABLE[TRIANGLE].push_back(4.0f * x);
    } else if (x < 0.75f) {
      WAVE_TABLE[TRIANGLE].push_back(2.0f - 4.0f * x);
    } else {
      WAVE_TABLE[TRIANGLE].push_back(4.0f * x - 4.0f);
    }
  }

  for (int i = 0; i < size; i++) {
    if (i < size / 2) {
      WAVE_TABLE[SQUARE].push_back(1.0f);
    } else {
      WAVE_TABLE[SQUARE].push_back(-1.0f);
    }
  }

  for (int i = 0; i < size; i++) {
    WAVE_TABLE[SAWTOOTH].push_back(2.0f * ((float)i / size) - 1.0f);
  }

  unsigned lfsr = 0xAFE1u;
  unsigned bit;

  for (int i = 0; i < size; i++) {
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
    lfsr = (lfsr >> 1) | (bit << 15);
    WAVE_TABLE[NOISE].push_back(2.0f * (lfsr / (float)0xFFFF) - 1.0f);
  }
}

auto WaveTable::get(WaveType waveType, float step) -> float {
  return WAVE_TABLE[waveType][(int)step];
}

auto WaveTable::getSize() -> int { return WAVE_TABLE[TRIANGLE].size(); }
};  // namespace sid