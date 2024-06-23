#include "Sequencer.h"

#include <memory>

#include "latebit/sid/synth/Configuration.h"
#include "latebit/sid/synth/Tune.h"
#include "latebit/sid/synth/Wavetable.h"
#include "test/lib/test.h"

using namespace sid;

void envelope() {
  auto e = Envelope();

  e.attack();
  assertEq("starts at 0", e.getValue(), 0.0);
  assertEq("starts in attack", e.getState(), EnvelopeState::ATTACK);

  for (int i = 0; i <= 100; i++) e.process();
  assertEq("attack completes", e.getValue(), 1.0);
  assertEq("moves to decay", e.getState(), EnvelopeState::DECAY);

  for (int i = 0; i <= 500; i++) e.process();
  assertEq("decay completes", e.getValue(), e.getSustainLevel());
  assertEq("moves to sustain", e.getState(), EnvelopeState::SUSTAIN);

  e.release();
  assertEq("moves to release", e.getState(), EnvelopeState::RELEASE);

  for (int i = 0; i <= 500; i++) e.process();
  assertEq("release completes", e.getValue(), 0.0);
  assertEq("done", e.getState(), EnvelopeState::DONE);
}

void getSamples() {
  vector<unique_ptr<Track>> tracks;
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("C#4---"));
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("C#4---"));
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("C#4---"));
  tracks.back()->push_back(Note::fromSymbol("D-4---"));
  Tune tune(10, 4, 2, std::move(tracks));

  Sequencer s;
  s.loadTune(&tune);
  s.play();

  s.getNextSample();
  assertEq("advances sample", s.getCurrentSampleIndex(), 2);

  assertEq("does not advance note (0)", s.getCurrentTick(0), 0);
  assertEq("does not advance note (1)", s.getCurrentTick(1), 0);
  assertEq("does not advance note (2)", s.getCurrentTick(2), 0);

  // advance until the envelope starts closing
  for (int i = 2; i < s.getSamplesPerTick() - ENVELOPE_RELEASE_SAMPLES; i++) {
    s.getNextSample();
  }

  // close the envelope
  s.getNextSample();
  assertEq("releases envelope (0)", s.getEnvelope(0)->getState(),
           EnvelopeState::RELEASE);
  assertEq("releases envelope (1)", s.getEnvelope(1)->getState(),
           EnvelopeState::RELEASE);
  assertEq("releases envelope (2)", s.getEnvelope(2)->getState(),
           EnvelopeState::RELEASE);

  // advance until the end of the tick
  for (int i = s.getSamplesPerTick() - ENVELOPE_RELEASE_SAMPLES + 1;
       i < s.getSamplesPerTick(); i++) {
    s.getNextSample();
  }

  // start the next note
  s.getNextSample();
  for (int i = 0; i < 3; i++) {
    assertEq("advances notes", s.getCurrentTick(i), 1);
    assertEq("resets envelope", s.getEnvelope(i)->getState(),
             EnvelopeState::ATTACK);
  }
}

void emptyTracks() {
  Sequencer s;
  vector<unique_ptr<Track>> tracks;
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("C#4---"));
  tracks.push_back(make_unique<Track>());
  Tune tune(10, 4, 2, std::move(tracks));
  s.loadTune(&tune);
  s.play();
  for (int i = 0; i < 100; i++) {
    s.getNextSample();
  }

  assertEq("does not crash", true, true);
}

auto main() -> int {
  test("envelope", envelope);
  test("getSamples", getSamples);
  test("initializes the wavetable", []() {
    auto s = Sequencer();
    assertEq("initializes the wavetable", WaveTable::getSize(),
             Configuration::getBufferSize());
  });
  test("can handle empty tracks", emptyTracks);

  return report();
}
