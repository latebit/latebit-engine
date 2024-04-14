#include "oscillator.h"

#include <array>
#include <cstdlib>

#include "utils.h"

using namespace std;

const int WAVE_TABLE_SIZE = BUFFER_SIZE;
array<array<float, BUFFER_SIZE>, WAVES> waveTable;
array<float, 96> NOTE_TO_FREQUENCY = {
  16.35,   17.32,   18.35,   19.45,   20.60,   21.83,
  23.12,   24.50,   25.96,   27.50,   29.14,   30.87,

  32.7,    34.65,   36.71,   38.89,   41.20,   43.65,
  46.25,   49,      51.91,   55,      58.27,   61.74,

  65.41,   69.23,   73.42,   77.78,   82.41,   87.31,
  92.5,    98,      103.83,  110,     116.54,  123.47,

  130.81,  138.59,  146.83,  155.56,  164.81,  174.61,
  185,     196,     207.65,  220,     233.08,  246.94,

  261.63,  277.18,  293.66,  311.12,  329.63,  311.13,
  369.99,  392,     415.30,  440,     466.16,  493.88,

  523.25,  554.37,  587.33,  622.25,  659.25,  698.46,
  734,     783.99,  830.61,  880,     932.33,  987.77,

  1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91,
  1471,    1567.98, 1661.22, 1760,    1864.66, 1975.53,

  2093,    2217.46, 2349.32, 2489.02, 2637.02, 2793.83,
  2959,    3135.96, 3322.44, 3520,    3729.31, 3951.07};

namespace sid {

bool initialized = false;
void initialize() {
  // Sine wave
  for (int i = 0; i < WAVE_TABLE_SIZE; i++) {
    float x = (float)i / WAVE_TABLE_SIZE;
    if (x < 0.25f) {
      waveTable[TRIANGLE][i] = 4.0f * x;
    } else if (x < 0.75f) {
      waveTable[TRIANGLE][i] = 2.0f - 4.0f * x;
    } else {
      waveTable[TRIANGLE][i] = 4.0f * x - 4.0f;
    }
  }

  // Square wave
  for (int i = 0; i < WAVE_TABLE_SIZE; i++) {
    if (i < BUFFER_SIZE / 2) {
      waveTable[SQUARE][i] = 1.0f;
    } else {
      waveTable[SQUARE][i] = -1.0f;
    }
  }

  // Sawtooth wave
  for (int i = 0; i < WAVE_TABLE_SIZE; i++) {
    waveTable[SAWTOOTH][i] = 2.0f * ((float)i / WAVE_TABLE_SIZE) - 1.0f;
  }

  // Noise wave
  for (int i = 0; i < WAVE_TABLE_SIZE; i++) {
    waveTable[NOISE][i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
  }
}

Oscillator::Oscillator(float frequency) {
  if (!initialized) {
    initialize();
    initialized = true;
  }

  this->stepSize = frequency / SAMPLE_RATE * WAVE_TABLE_SIZE;
}

Oscillator::~Oscillator() {}

auto Oscillator::oscillate() -> float {
  this->currentStep += this->effect.processFrequency(this->stepSize);

  if (this->currentStep >= BUFFER_SIZE) {
    this->currentStep -= BUFFER_SIZE;
  }

  float sample = waveTable[this->waveType][(int)this->currentStep];
  return this->effect.processVolume(sample) * this->volume;
}

void Oscillator::setFrequency(float frequency) {
  this->stepSize = frequency / SAMPLE_RATE * WAVE_TABLE_SIZE;
  this->currentStep = 0;
}

void Oscillator::setPitch(sid_byte note) {
  this->setFrequency(NOTE_TO_FREQUENCY[note]);
}

void Oscillator::setVolume(float volume) { this->volume = clamp(volume, 0, 1); }
void Oscillator::setWave(WaveType wave) { this->waveType = wave; }
void Oscillator::setEffect(EffectType effect) {
  switch (effect) {
    case DROP:
      this->effect = DROP_EFFECT;
      return;
    case SLIDE:
      this->effect = SLIDE_EFFECT;
      return;
    case FADEIN:
      this->effect = FADEIN_EFFECT;
      return;
    case FADEOUT:
      this->effect = FADEOUT_EFFECT;
      return;
    default:
    case NONE:
      this->effect = NONE_EFFECT;
      return;
  }
}
auto Oscillator::getWaveType() -> WaveType { return this->waveType; }
auto Oscillator::getEffectType() -> EffectType { return this->effect.type; }
auto Oscillator::getVolume() -> float { return this->volume; }
auto Oscillator::getCurrentStep() -> float { return this->currentStep; }
auto Oscillator::getStepSize() -> float { return this->stepSize; }

const Effect DROP_EFFECT = Effect(DROP, 0.9999, 1);
const Effect SLIDE_EFFECT = Effect(SLIDE, 1.00005, 1);
const Effect FADEIN_EFFECT = Effect(FADEIN, 0.0001, 0);
const Effect FADEOUT_EFFECT = Effect(FADEOUT, -0.0001, 1);
const Effect NONE_EFFECT = Effect(NONE, 0, 0);

Effect::Effect(EffectType type, float amount, float previous)
  : amount(amount), previous(previous), type(type) {}

auto Effect::processFrequency(float step) -> float {
  if (this->type == DROP || this->type == SLIDE) {
    step *= this->previous;
    this->previous *= this->amount;
  }
  return step;
}

auto Effect::processVolume(float sample) -> float {
  if (this->type == FADEIN || this->type == FADEOUT) {
    sample *= this->previous;
    this->previous = clamp(this->previous + this->amount, 0, 1);
  }
  return sample;
}

}  // namespace sid