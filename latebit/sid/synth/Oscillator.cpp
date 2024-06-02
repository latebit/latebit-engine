#include "Oscillator.h"

#include <cstdio>
#include <cstdlib>

#include "latebit/sid/synth/Configuration.h"
#include "latebit/sid/synth/Wavetable.h"
#include "latebit/utils/Math.h"

using namespace std;
using namespace sid;

array<float, 96> NOTE_TO_FREQUENCY = {
  16.351598f,   17.323914f,   18.354048f,   19.445436f,   20.601722f,
  21.826764f,   23.124651f,   24.499715f,   25.956544f,   27.5f,
  29.135235f,   30.867706f,   32.703196f,   34.647829f,   36.708096f,
  38.890873f,   41.203445f,   43.653529f,   46.249303f,   48.999429f,
  51.913087f,   55.0f,        58.27047f,    61.735413f,   65.406391f,
  69.295658f,   73.416192f,   77.781746f,   82.406889f,   87.307058f,
  92.498606f,   97.998859f,   103.826174f,  110.0f,       116.54094f,
  123.470825f,  130.812783f,  138.591315f,  146.832384f,  155.563492f,
  164.813778f,  174.614116f,  184.997211f,  195.997718f,  207.652349f,
  220.0f,       233.081881f,  246.941651f,  261.625565f,  277.182631f,
  293.664768f,  311.126984f,  329.627557f,  349.228231f,  369.994423f,
  391.995436f,  415.304698f,  440.0f,       466.163762f,  493.883301f,
  523.251131f,  554.365262f,  587.329536f,  622.253967f,  659.255114f,
  698.456463f,  739.988845f,  783.990872f,  830.609395f,  880.0f,
  932.327523f,  987.766603f,  1046.502261f, 1108.730524f, 1174.659072f,
  1244.507935f, 1318.510228f, 1396.912926f, 1479.977691f, 1567.981744f,
  1661.21879f,  1760.0f,      1864.655046f, 1975.533205f, 2093.004522f,
  2217.461048f, 2349.318143f, 2489.01587f,  2637.020455f, 2793.825851f,
  2959.955382f, 3135.963488f, 3322.437581f, 3520.0f,      3729.310092f,
  3951.06641f};

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

}  // namespace sid