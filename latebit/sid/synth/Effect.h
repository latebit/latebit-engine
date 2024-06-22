#pragma once

namespace sid {

namespace EffectType {
// Effects supported by the oscillator
enum EffectType {
  // No effect
  NONE,
  // Drop the frequency down
  DROP,
  // Slide the frequency up
  SLIDE,
  // Fade in the volume
  FADEIN,
  // Fade out the volume
  FADEOUT
};
}  // namespace EffectType

// Effects modulate the frequency or volume of the oscillator
class Effect {
  // How much the effect will change the frequency or volume
  float amount = 0;
  // A value used to store the previous sample when effects need memory
  float previous = 0;

 public:
  Effect(EffectType::EffectType type, float amount, float previous);

  // The type of effect
  EffectType::EffectType type = EffectType::NONE;
  // Modulates the frequency based on the current effect returning the adjusted
  // step size
  auto processFrequency(float step) -> float;
  // Modulates the volume based on the current effect returning the adjusted
  // volume
  auto processVolume(float sample) -> float;
};

extern const Effect DROP_EFFECT;
extern const Effect SLIDE_EFFECT;
extern const Effect FADEIN_EFFECT;
extern const Effect FADEOUT_EFFECT;
extern const Effect NONE_EFFECT;
}  // namespace sid
