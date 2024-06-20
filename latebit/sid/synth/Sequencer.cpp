#include "Sequencer.h"

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "Configuration.h"
#include "Envelope.h"
#include "Note.h"
#include "Oscillator.h"
#include "Tune.h"

using namespace std;

namespace sid {
// How many samples before the end of the note should the envelope start
// releasing. This number is very arbitrary and based on what "sounds good".
// Exported for testing purposes.
const int ENVELOPE_RELEASE_SAMPLES = Configuration::getSampleRate() / 100;

void Sequencer::setNoteForTrack(Note n, int track) {
  auto& o = oscillators.at(track);
  auto& e = envelopes.at(track);

  o->setPitch(n.getPitch());
  o->setVolume(n.getVolume());
  o->setEffect(n.getEffect());
  o->setWave(n.getWave());
  e->attack();
}

auto Sequencer::loadTune(Tune* t) -> int {
  if (this->tune) return -1;

  this->tune = t;
  // Start currentSample at one else the first note will be skipped because the
  // currentSample is used to determine when to move to the next note.
  // Using zero means we will move to the next note on the first sample (see
  // getNextSampleForChannel)
  this->currentSample = 1;
  this->samplesPerTick =
    Configuration::getSampleRate() / (t->getBpm() * t->getTicksPerBeat()) * 60;
  this->currentTick.resize(t->getTracksCount());
  this->oscillators.resize(t->getTracksCount());
  this->envelopes.resize(t->getTracksCount());

  int maxTrackLength = 0;
  for (int i = 0; i < t->getTracksCount(); i++) {
    this->currentTick[i] = 0;
    this->envelopes[i] = make_unique<Envelope>();
    this->oscillators[i] = make_unique<Oscillator>(0);
    auto& track = t->getTrack(i);
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

  this->tune = nullptr;
  this->currentSample = 1;
  this->samplesPerTick = 0;
  this->totalSamples = 0;
  this->currentTick.clear();
  this->oscillators.clear();
  this->envelopes.clear();

  return 0;
}

auto Sequencer::play() -> int {
  if (!this->tune) return -1;

  // Make sure the last note is set as the current note, else the first note is
  // skipped when you resume playing and you have paused just after the tick
  for (int i = 0; i < this->tune->getTracksCount(); i++) {
    auto& track = this->tune->getTrack(i);
    if (track->empty()) continue;

    Note lastNote = track->at(this->getCurrentTick(i));
    this->setNoteForTrack(lastNote, i);
  }

  // Update samples per tick in case the bpm or the ticks per beat has changed
  this->samplesPerTick =
    Configuration::getSampleRate() /
    (this->tune->getBpm() * this->tune->getTicksPerBeat()) * 60;

  // Consequently, update also the total samples
  int maxTrackLength = 0;
  for (int i = 0; i < this->tune->getTracksCount(); i++) {
    auto& track = this->tune->getTrack(i);
    maxTrackLength = max(maxTrackLength, (int)track->size());
  }
  this->totalSamples = maxTrackLength * this->samplesPerTick;

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

  this->currentSample = 1;
  for (int i = 0; i < this->tune->getTracksCount(); i++) {
    this->currentTick[i] = 0;
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

  const int shouldMoveToNextNote =
    this->currentSample % this->samplesPerTick == 0;
  const int shouldStopEnvelope =
    (this->currentSample + ENVELOPE_RELEASE_SAMPLES) % this->samplesPerTick ==
    0;
  float result = 0;
  const int tracks = this->tune->getTracksCount();

  // Plays the current sample in every track
  for (int trackIndex = 0; trackIndex < tracks; trackIndex++) {
    auto& track = this->tune->getTrack(trackIndex);
    if (track->empty()) continue;

    const auto& envelope = this->envelopes[trackIndex];
    const auto& oscillator = this->oscillators[trackIndex];

    const int currentTick = this->currentTick[trackIndex];
    const int nextTick = (currentTick + 1) % track->size();

    const Note next = track->at(nextTick);
    const bool isChangingNotes = next.getType() != NoteType::Continue;

    if (shouldStopEnvelope && isChangingNotes) {
      envelope->release();
    }

    if (shouldMoveToNextNote) {
      this->currentTick[trackIndex] = nextTick;

      if (isChangingNotes) {
        this->setNoteForTrack(next, trackIndex);
      }
    }

    result += oscillator->oscillate() * envelope->process() / (float)tracks;
  }

  if (this->currentSample >= this->totalSamples) {
    this->currentSample = 0;
    if (!this->loop) {
      this->stop();
    }
  }

  this->currentSample++;

  return result;
}

auto Sequencer::getCurrentSampleIndex() const -> int {
  return this->currentSample;
}
auto Sequencer::getCurrentTick(int channel) const -> int {
  return this->currentTick.at(channel);
}

auto Sequencer::getSamplesPerTick() const -> int {
  return this->samplesPerTick;
}

auto Sequencer::getEnvelope(int channel) const -> const unique_ptr<Envelope>& {
  return this->envelopes.at(channel);
}

auto Sequencer::setLoop(bool loop) -> void { this->loop = loop; }
auto Sequencer::isLooping() const -> bool { return this->loop; }
auto Sequencer::isPlaying() const -> bool { return this->playing; }
auto Sequencer::getCurrentTune() const -> const Tune* { return this->tune; }
}  // namespace sid