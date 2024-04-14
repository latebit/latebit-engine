#include "oscillator.h"

#include <array>

#include "../../../test/lib/run.h"
#include "../../../test/lib/test.h"
#include "utils.h"

using namespace sid;

void init() {
  auto o = Oscillator(440);
  assert_int("waveType should be TRIANGLE", o.getWaveType(), TRIANGLE);
  assert_int("effect should be NONE", o.getEffectType(), NONE);
  assert_float("step should be 0", o.getCurrentStep(), 0);
  assert_float("stepSize should be 0", o.getStepSize(), 40.867119);
}

void pitch() {
  auto o = Oscillator(100);
  o.setPitch(69);
  assert_int("reset step to 0", o.getCurrentStep(), 0);
  assert_float("sets correct stepSize", o.getStepSize(), 81.734240);
}

void effect() {
  auto o = Oscillator(100);
  o.setEffect(DROP);
  assert_int("sets effect type to DROP", o.getEffectType(), DROP);
  o.setEffect(SLIDE);
  assert_int("sets effect type to SLIDE", o.getEffectType(), SLIDE);
  o.setEffect(FADEIN);
  assert_int("sets effect type to FADEIN", o.getEffectType(), FADEIN);
  o.setEffect(FADEOUT);
  assert_int("sets effect type to FADEOUT", o.getEffectType(), FADEOUT);
  o.setEffect(NONE);
  assert_int("sets effect type to NONE", o.getEffectType(), NONE);
}

void frequency() {
  Effect e = DROP_EFFECT;
  array<float, 2> result = {};
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assert_lt("reduces the frequency", result[0], 100);
  assert_lt("reduces the frequency", result[1], result[0]);

  e = SLIDE_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assert_lt("increases the frequency", 100, result[0]);
  assert_lt("increases the frequency", result[0], result[1]);

  e = FADEIN_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assert_float("doesn't change the frequency", result[0], 100);
  assert_float("doesn't change the frequency", result[1], result[0]);

  e = FADEOUT_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assert_float("doesn't change the frequency", result[0], 100);
  assert_float("doesn't change the frequency", result[1], result[0]);

  e = NONE_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assert_float("doesn't change the frequency", result[0], 100);
  assert_float("doesn't change the frequency", result[1], result[0]);
}

void volume() {
  Effect e = FADEIN_EFFECT;
  array<float, 3> result = {};

  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assert_lt("increases the volume", result[0], result[1]);
  assert_lt("increases the volume", result[1], result[2]);

  e = FADEOUT_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assert_lt("decreases the volume", result[1], result[0]);
  assert_lt("decreases the volume", result[2], result[1]);

  e = DROP_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assert_float("doesn't change the volume", result[1], 0.5);
  assert_float("doesn't change the volume", result[2], result[1]);

  e = NONE_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assert_float("doesn't change the volume", result[1], 0.5);
  assert_float("doesn't change the volume", result[2], result[1]);

  e = SLIDE_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assert_float("doesn't change the volume", result[1], 0.5);
  assert_float("doesn't change the volume", result[2], result[1]);
}

void oscillations() {
  // Purposely chosing a slow frequency so that we don't have to deal with
  // periodicity
  const int FREQUENCY = 100;
  Oscillator o1 = Oscillator(FREQUENCY);
  Oscillator o2 = Oscillator(FREQUENCY);
  o1.setEffect(SLIDE);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    o1.oscillate();
    o2.oscillate();
  }
  assert_lt("applies effect to frequency", o1.oscillate(), o2.oscillate());

  o1 = Oscillator(FREQUENCY);
  o2 = Oscillator(FREQUENCY);
  o1.setEffect(FADEIN);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    o1.oscillate();
    o2.oscillate();
  }

  assert_lt("applies effect to volume", o1.oscillate(), o2.oscillate());
}

auto main() -> int {
  return run([]() -> void {
    suite("Oscillator", []() {
      test("init", init);
      test("pitch", pitch);
      test("effect", effect);
      test("oscillations", oscillations);
    });

    suite("Effect", []() {
      test("frequency", frequency);
      test("volume", volume);
    });
  });
}
