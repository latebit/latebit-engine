#pragma once

namespace sid {
// Effects supported by the oscillator
enum EffectType { NONE, DROP, SLIDE, FADEIN, FADEOUT };

// Effects modulate the frequency or volume of the oscillator
class Effect {
  // How much the effect will change the frequency or volume
  float amount = 0;
  // A value used to store the previous sample when effects need memory
  float previous = 0;

 public:
  Effect(EffectType type, float amount, float previous);

  // The type of effect
  EffectType type = NONE;
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
