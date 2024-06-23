#pragma once

#include <memory>
#include <vector>

#include "Configuration.h"
#include "Oscillator.h"
#include "Tune.h"

namespace sid {

namespace EnvelopeState {
enum EnvelopeState { ATTACK, DECAY, SUSTAIN, RELEASE, DONE };
}

// Defines a volume envelope. Its value is between 0 and 1 where 0 is
// silence and 1 is full volume associated with the Note being played
class Envelope {
 public:
  Envelope() = default;
  ~Envelope() = default;
  // Starts the envelope
  void attack();
  // Initiates the release phase of the envelope
  void release();
  // Resets the envelope to the done state
  void done();
  // Processes the envelope for the next sample and returns the value of the
  // envelope
  auto process() -> float;
  // Returns the current value of the envelope
  [[nodiscard]] auto getValue() const -> float;
  // Returns the current state of the envelope
  [[nodiscard]] auto getState() const -> EnvelopeState::EnvelopeState;
  // Returns the level of volume to sustain after the decay phase
  [[nodiscard]] auto getSustainLevel() const -> float;

 private:
  // The current state of the envelope
  EnvelopeState::EnvelopeState state = EnvelopeState::DONE;
  // The current value of the envelope
  float value = 0;
  // How much volume to increase per sample in the attack phase
  float attackPerSample = 0.01f;
  // How much volume to decrease per sample in the decay phase
  float decayPerSample = 0.001f;
  // The level of volume to sustain after the decay phase
  float sustainLevel = 0.5f;
  // How much volume to decrease per sample in the release phase1
  float releasePerSample = 0.01f;
};
}  // namespace sid