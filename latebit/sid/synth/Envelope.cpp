#include <algorithm>
#include <memory>
#include <stdexcept>

#include "Configuration.h"
#include "Note.h"
#include "Oscillator.h"
#include "Sequencer.h"
#include "Tune.h"

using namespace std;

namespace sid {
void Envelope::attack() {
  state = EnvelopeState::ATTACK;
  value = 0;
}

void Envelope::release() { state = EnvelopeState::RELEASE; }

void Envelope::done() {
  state = EnvelopeState::DONE;
  value = 0;
}

auto Envelope::process() -> float {
  switch (this->state) {
    case EnvelopeState::ATTACK:
      this->value += this->attackPerSample;
      if (this->value >= 1) {
        this->value = 1;
        this->state = EnvelopeState::DECAY;
      }
      break;
    case EnvelopeState::DECAY:
      this->value -= this->decayPerSample;
      if (this->value <= this->sustainLevel) {
        this->value = this->sustainLevel;
        this->state = EnvelopeState::SUSTAIN;
      }
      break;
    default:
    case EnvelopeState::SUSTAIN:
      break;
    case EnvelopeState::RELEASE:
      this->value -= this->releasePerSample;
      if (this->value <= 0) {
        this->value = 0;
        this->state = EnvelopeState::DONE;
      }
      break;
  }

  return this->value;
}

auto Envelope::getValue() const -> float { return this->value; }
auto Envelope::getState() const -> EnvelopeState::EnvelopeState {
  return this->state;
}
auto Envelope::getSustainLevel() const -> float { return this->sustainLevel; }
}  // namespace sid