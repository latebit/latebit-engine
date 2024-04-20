#pragma once

namespace sid {

// This is used to initialize the wave table
#define WAVES 4

// Waves supported by the oscillator
enum WaveType { TRIANGLE, SQUARE, SAWTOOTH, NOISE };
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

// The oscillator is the main structure that produces sound. It contains the
// current step in the wave table, the step size used to advance in the wave
// table, the volume of the oscillator, the wave type used to produce the sound
// and the effect applied to the oscillator to modulate the frequency or volume.
class Oscillator {
 public:
  Oscillator(float frequency);
  ~Oscillator() = default;

  // Advances the oscillator by one step returning the current sample
  auto oscillate() -> float;

  // Resets the oscillator to the initial state
  void reset();

  // Sets the pitch of the oscillator
  void setPitch(int note);
  // Sets the volume of the oscillator
  void setVolume(float volume);
  // Sets the wave of the oscillator
  void setWave(WaveType wave);
  // Sets the effect of the oscillator
  void setEffect(EffectType effect);

  // Returns the wave of the oscillator
  auto getWaveType() -> WaveType;
  // Returns the effect of the oscillator
  auto getEffectType() -> EffectType;
  // Returns the volume of the oscillator
  auto getVolume() -> float;
  // Returns the current step in the wave table
  auto getCurrentStep() -> float;
  // Returns the step size used to advance in the wave table
  auto getStepSize() -> float;

 private:
  // The current step in the wave table
  float currentStep = 0;
  // The step size used to advance in the wave table
  float stepSize = 0;
  // The volume of the oscillator
  float volume = 0.5;
  // The wave type used to produce the sound
  // This also maps to the wave table
  WaveType waveType = TRIANGLE;
  // The effect applied to the oscillator to modulate the frequency or volume
  Effect effect = NONE_EFFECT;

  // Sets the frequency of the oscillator. Used by setPitch
  void setFrequency(float frequency);
};
}  // namespace sid
