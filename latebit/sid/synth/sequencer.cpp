#include "sequencer.h"

#include <algorithm>
#include <memory>

#include "configuration.h"
#include "oscillator.h"
#include "track.h"
#include "tune.h"

using namespace std;

namespace sid {
const int ENVELOPE_RELEASE_SAMPLES = Configuration::getSampleRate() / 100;

void Envelope::attack() {
  state = ATTACK;
  value = 0;
}

void Envelope::release() { state = RELEASE; }

void Envelope::done() {
  state = DONE;
  value = 0;
}

auto Envelope::process() -> float {
  switch (this->state) {
    case ATTACK:
      this->value += this->attackPerSample;
      if (this->value >= 1) {
        this->value = 1;
        this->state = DECAY;
      }
      break;
    case DECAY:
      this->value -= this->decayPerSample;
      if (this->value <= this->sustainLevel) {
        this->value = this->sustainLevel;
        this->state = SUSTAIN;
      }
      break;
    default:
    case SUSTAIN:
      break;
    case RELEASE:
      this->value -= this->releasePerSample;
      if (this->value <= 0) {
        this->value = 0;
        this->state = DONE;
      }
      break;
  }

  return this->value;
}

auto Envelope::getValue() const -> float { return this->value; }
auto Envelope::getState() const -> EnvelopeState { return this->state; }
auto Envelope::getSustainLevel() const -> float { return this->sustainLevel; }

void Sequencer::setNoteForTrack(Note n, int track) {
  auto& o = oscillators.at(track);
  auto& e = envelopes.at(track);

  o->setPitch(n.getPitch());
  o->setVolume(n.getVolume());
  o->setEffect(n.getEffect());
  o->setWave(n.getWave());
  e->attack();
}

auto Sequencer::loadTune(shared_ptr<Tune> tune) -> int {
  if (this->tune) return -1;

  this->tune = tune;
  Tune* t = tune.get();
  // Start currentSample at one else the first note will be skipped because the
  // currentSample is used to determine when to move to the next note.
  // Using zero means we will move to the next note on the first sample (see
  // getNextSampleForChannel)
  this->currentSample = 1;
  this->samplesPerTick =
    Configuration::getSampleRate() / (t->getBpm() * t->getTicksPerBeat()) * 60;
  this->currentNoteIndex.resize(t->getTracksCount());
  this->oscillators.resize(t->getTracksCount());
  this->envelopes.resize(t->getTracksCount());

  int maxTrackLength = 0;
  for (int i = 0; i < t->getTracksCount(); i++) {
    this->currentNoteIndex[i] = 0;
    this->envelopes[i] = make_unique<Envelope>();
    this->oscillators[i] = make_unique<Oscillator>(0);
    shared_ptr<Track> track = t->getTrack(i);
    maxTrackLength = max(maxTrackLength, (int)track->size());

    // This is used to allow the first note of each track to be executed
    // else the first note will be skipped until there is a change note event
    try {
      Note firstNote = track->at(0);
      this->setNoteForTrack(firstNote, i);
    } catch (const out_of_range& e) {
      // Do nothing
    }
  }

  this->totalSamples = maxTrackLength * this->samplesPerTick;

  return 0;
}

auto Sequencer::unloadTune() -> int {
  if (!this->tune) return -1;

  this->tune.reset();
  this->tune = nullptr;
  this->currentSample = 0;
  this->samplesPerTick = 0;
  this->totalSamples = 0;
  this->currentNoteIndex.clear();
  this->oscillators.clear();
  this->envelopes.clear();

  return 0;
}

auto Sequencer::play() -> int {
  if (!this->tune) return -1;
  this->playing = true;
  return 0;
}

auto Sequencer::pause() -> int {
  if (!this->tune) return -1;
  this->playing = false;
  return 0;
}

auto Sequencer::stop() -> int {
  if (!this->tune) return -1;
  this->playing = false;
  this->loop = false;

  for (int i = 0; i < this->tune->getTracksCount(); i++) {
    this->currentNoteIndex[i] = 0;
    this->envelopes[i]->done();
    this->oscillators[i]->reset();
  }

  return 0;
}

Sequencer::Sequencer(int bufferSize) {
  Configuration::setBufferSize(bufferSize);
}

Sequencer::~Sequencer() {
  if (this->tune) {
    this->unloadTune();
  }
}

auto Sequencer::getNextSample() -> float {
  if (!this->tune || !this->playing) {
    return 0;
  }

  int shouldMoveToNextNote = this->currentSample % this->samplesPerTick == 0;
  int shouldStopEnvelope =
    (this->currentSample + ENVELOPE_RELEASE_SAMPLES) % this->samplesPerTick ==
    0;
  float result = 0;

  // Plays the current sample in every channel
  for (int channel = 0; channel < this->tune->getTracksCount(); channel++) {
    shared_ptr<Track> track = this->tune->getTrack(channel);
    auto& envelope = this->envelopes[channel];
    auto& oscillator = this->oscillators[channel];

    int currentNoteIndex = this->currentNoteIndex[channel];
    int newNoteIndex = (currentNoteIndex + 1) % track->size();

    Note current = track->at(currentNoteIndex);
    Note next = track->at(newNoteIndex);
    bool isChangingNotes = !next.isSame(current);

    if (shouldStopEnvelope && isChangingNotes) {
      envelope->release();
    }

    if (shouldMoveToNextNote) {
      this->currentNoteIndex[channel] = newNoteIndex;

      if (isChangingNotes) {
        this->setNoteForTrack(next, channel);
      }
    }

    result += oscillator->oscillate() * envelope->process();
  }

  this->currentSample++;

  if (this->currentSample >= this->totalSamples) {
    this->currentSample = 0;
    if (!this->loop) {
      this->stop();
    }
  }

  return result / this->tune->getTracksCount();
}

auto Sequencer::getCurrentSampleIndex() const -> int {
  return this->currentSample;
}
auto Sequencer::getCurrentNoteIndex(int trackIndex) const -> int {
  return this->currentNoteIndex[trackIndex];
}

auto Sequencer::getSamplesPerTick() const -> int {
  return this->samplesPerTick;
}
auto Sequencer::getEnvelope(int trackIndex) const
  -> const unique_ptr<Envelope>& {
  return this->envelopes[trackIndex];
}

auto Sequencer::setLoop(bool loop) -> void { this->loop = loop; }
auto Sequencer::isLooping() const -> bool { return this->loop; }
auto Sequencer::getCurrentTune() const -> shared_ptr<Tune> {
  return this->tune;
}
}  // namespace sid