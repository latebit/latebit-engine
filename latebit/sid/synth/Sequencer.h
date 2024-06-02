#pragma once

#include <memory>
#include <vector>

#include "Configuration.h"
#include "Envelope.h"
#include "Oscillator.h"
#include "Tune.h"

namespace sid {
extern const int ENVELOPE_RELEASE_SAMPLES;

// The sequencer is responsible for playing a tune. It has a set of oscillators
// and envelopes that are used to play the notes in the tune. It keeps track of
// the current note being played and the current tick for each track. It also
// keeps track of the current sample being played within a single tick.
class Sequencer {
 private:
  // Keeps track of the current sample being played within a single tick
  int currentSample = 0;

  // Keeps track of the index of the current note for each track
  vector<int> currentTick = {};

  // How many samples are there in a single tick
  int samplesPerTick = 0;

  // Total amount of samples present in one execution of the track. Used to
  // determine if we should stop playing or loop
  int totalSamples = 0;

  // The tune to be played
  Tune* tune = nullptr;

  // The oscillators for each track
  vector<unique_ptr<Oscillator>> oscillators = {};

  // The envelopes for each track.
  // They are not customisable for now, we use them to have smooth transitions
  // across notes
  vector<unique_ptr<Envelope>> envelopes = {};

  // Whether the sequencer is playing or not
  bool playing = false;

  // True if the sequencer should be looping the current tune
  bool loop = false;

  // Updates oscillators and envelopes to play the note
  void setNoteForTrack(Note n, int trackIndex);

 public:
  // Creates a new sequencer
  Sequencer(int bufferSize = Configuration::getBufferSize());
  // Frees the memory associated with sequencer
  ~Sequencer();

  // Loads the given tune in the sequencer, allocating all the necessary
  // envelopes and oscillators. If a tune has been loaded before, you need to
  // call unloadTune first. Returns -1 for failure.
  auto loadTune(Tune* t) -> int;

  // Unloads the current tune and frees the resources associated with it. It
  // doesn't change oscillators and envelopes though, as they will be
  // reinitialised in the next loadTune call.
  auto unloadTune() -> int;

  // Sets the sequencer to play the loaded tune. Call getNextSampleForChannel to
  // get samples. This function is idempotent: multiple calls are deemed as
  // succesful but won't do anything.
  // It returns -1 if the tune hasn't been loaded yet.
  auto play() -> int;

  // Stops the sequencer from playing the tune. Subsequent
  // getNextSampleForChannel calls will retun 0. This function is idempotent:
  // multiple calls are deemed as succesful but won't do anything. It returns -1
  // if the tune hasn't been loaded yet.
  auto stop() -> int;

  // Pauses the sequencer. Subsequent getNextSampleForChannel calls will retun
  // 0. This function is idempotent: multiple calls are deemed as succesful but
  // won't do anything. It returns -1 if the tune hasn't been loaded yet.
  auto pause() -> int;

  // Returns the next float32 sample for the given sequencer or 0 if the tune is
  // not playing. It gets the next sample from all the tracks, processes the
  // envelopes and returns the average of all the samples (i.e., the tracks are
  // mixed equally)
  auto getNextSample() -> float;

  // Returns the tick being played for the given channel
  [[nodiscard]] auto getCurrentTick(int channel) const -> int;

  // Returns the number of samples per tick
  [[nodiscard]] auto getSamplesPerTick() const -> int;

  // Sets the loop flag to the given value
  void setLoop(bool loop);

  // Returns the loop flag
  [[nodiscard]] auto isLooping() const -> bool;

  // Returns the play flag
  [[nodiscard]] auto isPlaying() const -> bool;

  // Returns the currently loaded tune
  [[nodiscard]] auto getCurrentTune() const -> const Tune*;

  // Returns the index of the current sample being played.
  // Exposed for testing purposes.
  [[nodiscard]] auto getCurrentSampleIndex() const -> int;

  // Returns the envelope for the given track
  [[nodiscard]] auto getEnvelope(int channel) const
    -> const unique_ptr<Envelope>&;
};
}  // namespace sid