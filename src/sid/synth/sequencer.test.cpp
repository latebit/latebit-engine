#include "sequencer.h"

#include <memory>

#include "../../../test/lib/run.h"
#include "../../../test/lib/test.h"
#include "sid/synth/tune.h"

using namespace sid;

void envelope() {
  auto e = Envelope();

  e.attack();
  assert_float("starts at 0", e.getValue(), 0);
  assert_int("starts in attack", e.getState(), ATTACK);

  for (int i = 0; i <= 100; i++) e.process();
  assert_float("attack completes", e.getValue(), 1);
  assert_int("moves to decay", e.getState(), DECAY);

  for (int i = 0; i <= 500; i++) e.process();
  assert_float("decay completes", e.getValue(), e.getSustainLevel());
  assert_int("moves to sustain", e.getState(), SUSTAIN);

  e.release();
  assert_int("moves to release", e.getState(), RELEASE);

  for (int i = 0; i <= 500; i++) e.process();
  assert_float("release completes", e.getValue(), 0);
  assert_int("done", e.getState(), DONE);
}

void getSamples() {
  shared_ptr<Tune> t(new Tune(3));
  Track one = {
    Note(48, 8, TRIANGLE, NONE),
    Note(49, 8, TRIANGLE, NONE),
  };
  Track two = {
    Note(48, 8, TRIANGLE, NONE),
    Note(49, 8, TRIANGLE, NONE),
  };
  Track three = {
    Note(48, 8, TRIANGLE, NONE),
    Note(49, 8, TRIANGLE, NONE),
    Note(50, 8, TRIANGLE, NONE),
  };
  t->getTrack(0)->insert(t->getTrack(0)->end(), one.begin(), one.end());
  t->getTrack(1)->insert(t->getTrack(1)->end(), two.begin(), two.end());
  t->getTrack(2)->insert(t->getTrack(2)->end(), three.begin(), three.end());
  t->setBeatsCount(2);

  Sequencer s;
  s.loadTune(t);
  s.play();

  s.getNextSample();
  assert_int("advances sample", s.getCurrentSampleIndex(), 2);

  assert_int("does not advance note (0)", s.getCurrentNoteIndex(0), 0);
  assert_int("does not advance note (1)", s.getCurrentNoteIndex(1), 0);
  assert_int("does not advance note (2)", s.getCurrentNoteIndex(2), 0);

  // advance until the envelope starts closing
  for (int i = 2; i < s.getSamplesPerTick() - ENVELOPE_RELEASE_SAMPLES; i++) {
    s.getNextSample();
  }

  // close the envelope
  s.getNextSample();
  assert_int("releases envelope (0)", s.getEnvelope(0)->getState(), RELEASE);
  assert_int("releases envelope (1)", s.getEnvelope(1)->getState(), RELEASE);
  assert_int("releases envelope (2)", s.getEnvelope(2)->getState(), RELEASE);

  // advance until the end of the tick
  for (int i = s.getSamplesPerTick() - ENVELOPE_RELEASE_SAMPLES + 1;
       i < s.getSamplesPerTick(); i++) {
    s.getNextSample();
  }

  // start the next note
  s.getNextSample();
  for (int i = 0; i < 3; i++) {
    assert_int("advances notes", s.getCurrentNoteIndex(i), 1);
    assert_int("resets envelope", s.getEnvelope(i)->getState(), ATTACK);
  }
}

auto main() -> int {
  return run([]() {
    test("envelope", envelope);
    test("getSamples", getSamples);
  });
}
