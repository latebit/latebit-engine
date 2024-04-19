#include "oscillator.h"

#include <array>

#include "../../../test/lib/test.h"
#include "sid/synth/config.h"

using namespace sid;

void init() {
  auto o = Oscillator(440);
  assertEq("waveType should be TRIANGLE", o.getWaveType(), TRIANGLE);
  assertEq("effect should be NONE", o.getEffectType(), NONE);
  assertEq("step should be 0", o.getCurrentStep(), 0.0);
  assertEq("stepSize should be 0", o.getStepSize(), 40.867119);
}

void pitch() {
  auto o = Oscillator(100);
  o.setPitch(69);
  assertEq("reset step to 0", o.getCurrentStep(), 0.0);
  assertEq("sets correct stepSize", o.getStepSize(), 81.734240);
}

void effect() {
  auto o = Oscillator(100);
  o.setEffect(DROP);
  assertEq("sets effect type to DROP", o.getEffectType(), DROP);
  o.setEffect(SLIDE);
  assertEq("sets effect type to SLIDE", o.getEffectType(), SLIDE);
  o.setEffect(FADEIN);
  assertEq("sets effect type to FADEIN", o.getEffectType(), FADEIN);
  o.setEffect(FADEOUT);
  assertEq("sets effect type to FADEOUT", o.getEffectType(), FADEOUT);
  o.setEffect(NONE);
  assertEq("sets effect type to NONE", o.getEffectType(), NONE);
}

void frequency() {
  Effect e = DROP_EFFECT;
  array<float, 2> result = {};
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertLessThan("reduces the frequency", result[0], 100);
  assertLessThan("reduces the frequency", result[1], result[0]);

  e = SLIDE_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertLessThan("increases the frequency", 100, result[0]);
  assertLessThan("increases the frequency", result[0], result[1]);

  e = FADEIN_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertEq("doesn't change the frequency", result[0], 100.0);
  assertEq("doesn't change the frequency", result[1], result[0]);

  e = FADEOUT_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertEq("doesn't change the frequency", result[0], 100.0);
  assertEq("doesn't change the frequency", result[1], result[0]);

  e = NONE_EFFECT;
  // Applying one more time due to float precision
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertEq("doesn't change the frequency", result[0], 100.0);
  assertEq("doesn't change the frequency", result[1], result[0]);
}

void volume() {
  Effect e = FADEIN_EFFECT;
  array<float, 3> result = {};

  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertLessThan("increases the volume", result[0], result[1]);
  assertLessThan("increases the volume", result[1], result[2]);

  e = FADEOUT_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertLessThan("decreases the volume", result[1], result[0]);
  assertLessThan("decreases the volume", result[2], result[1]);

  e = DROP_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertEq("doesn't change the volume", result[1], 0.5);
  assertEq("doesn't change the volume", result[2], result[1]);

  e = NONE_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertEq("doesn't change the volume", result[1], 0.5);
  assertEq("doesn't change the volume", result[2], result[1]);

  e = SLIDE_EFFECT;
  // Applying one more time due to float precision
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertEq("doesn't change the volume", result[1], 0.5);
  assertEq("doesn't change the volume", result[2], result[1]);
}

void oscillations() {
  // Purposely chosing a slow frequency so that we don't have to deal with
  // periodicity
  const int FREQUENCY = 100;
  auto o1 = Oscillator(FREQUENCY);
  auto o2 = Oscillator(FREQUENCY);
  o1.setEffect(SLIDE);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    o1.oscillate();
    o2.oscillate();
  }
  assertLessThan("applies effect to frequency", o1.oscillate(), o2.oscillate());

  o1 = Oscillator(FREQUENCY);
  o2 = Oscillator(FREQUENCY);
  o1.setEffect(FADEIN);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    o1.oscillate();
    o2.oscillate();
  }

  assertLessThan("applies effect to volume", o1.oscillate(), o2.oscillate());
}

auto main() -> int {
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
}
