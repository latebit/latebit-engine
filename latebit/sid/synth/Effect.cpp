#include <cstdio>
#include <cstdlib>

#include "Oscillator.h"
#include "latebit/utils/Math.h"

using namespace std;
using namespace sid;

namespace sid {
const Effect DROP_EFFECT = Effect(DROP, 0.99995, 1);
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