#include "Effect.h"
#include "latebit/utils/Math.h"

using namespace sid;

namespace sid {
const Effect DROP_EFFECT = Effect(EffectType::DROP, 0.99995, 1);
const Effect SLIDE_EFFECT = Effect(EffectType::SLIDE, 1.00005, 1);
const Effect FADEIN_EFFECT = Effect(EffectType::FADEIN, 0.0001, 0);
const Effect FADEOUT_EFFECT = Effect(EffectType::FADEOUT, -0.0001, 1);
const Effect NONE_EFFECT = Effect(EffectType::NONE, 0, 0);

Effect::Effect(EffectType::EffectType type, float amount, float previous)
  : amount(amount), previous(previous), type(type) {}

auto Effect::processFrequency(float step) -> float {
  if (this->type == EffectType::DROP || this->type == EffectType::SLIDE) {
    step *= this->previous;
    this->previous *= this->amount;
  }
  return step;
}

auto Effect::processVolume(float sample) -> float {
  if (this->type == EffectType::FADEIN || this->type == EffectType::FADEOUT) {
    sample *= this->previous;
    this->previous = lb::clamp(this->previous + this->amount, 0.0, 1.0);
  }
  return sample;
}

}  // namespace sid