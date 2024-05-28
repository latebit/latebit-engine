#include "oscillator.h"

#include <cstdio>
#include <cstdlib>

#include "latebit/sid/synth/configuration.h"
#include "latebit/sid/synth/wavetable.h"
#include "latebit/utils/Math.h"

using namespace std;
using namespace sid;

array<float, 96> NOTE_TO_FREQUENCY = {
  51.913087f,   55.0f,        58.27047f,    61.735413f,   65.406391f,

namespace sid {

Oscillator::Oscillator(float frequency) {
  this->stepSize =
    frequency / Configuration::getSampleRate() * Configuration::getBufferSize();
}

auto Oscillator::oscillate() -> float {
  this->currentStep += this->effect.processFrequency(this->stepSize);

  if (this->currentStep >= Configuration::getBufferSize()) {
    this->currentStep -= Configuration::getBufferSize();
  }

  float sample = WaveTable::get(this->waveType, this->currentStep);
  return this->effect.processVolume(sample) * this->volume;
}

void Oscillator::setFrequency(float frequency) {
  this->stepSize =
    frequency / Configuration::getSampleRate() * Configuration::getBufferSize();
  this->currentStep = 0;
}

void Oscillator::setPitch(int note) {
  this->setFrequency(NOTE_TO_FREQUENCY[note]);
}

void Oscillator::setVolume(float volume) {
  this->volume = clamp(volume, 0.0, 1.0);
}
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
auto Oscillator::reset() -> void {
  this->currentStep = 0;
  this->effect = NONE_EFFECT;
  this->volume = 0.5;
  this->waveType = TRIANGLE;
}

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
    this->previous = clamp(this->previous + this->amount, 0.0, 1.0);
  }
  return sample;
}

}  // namespace sid