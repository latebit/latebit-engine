#include "Oscillator.h"

#include "Configuration.h"
#include "test/lib/test.h"

using namespace sid;

void init() {
  auto o = Oscillator(440);
  assertEq("waveType should be TRIANGLE", o.getWaveType(), WaveType::TRIANGLE);
  assertEq("effect should be NONE", o.getEffectType(), EffectType::NONE);
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
  o.setEffect(EffectType::DROP);
  assertEq("sets effect type to DROP", o.getEffectType(), EffectType::DROP);
  o.setEffect(EffectType::SLIDE);
  assertEq("sets effect type to SLIDE", o.getEffectType(), EffectType::SLIDE);
  o.setEffect(EffectType::FADEIN);
  assertEq("sets effect type to FADEIN", o.getEffectType(), EffectType::FADEIN);
  o.setEffect(EffectType::FADEOUT);
  assertEq("sets effect type to FADEOUT", o.getEffectType(),
           EffectType::FADEOUT);
  o.setEffect(EffectType::NONE);
  assertEq("sets effect type to NONE", o.getEffectType(), EffectType::NONE);
}

void oscillations() {
  // Purposely chosing a slow frequency so that we don't have to deal with
  // periodicity
  const int FREQUENCY = 100;
  auto o1 = Oscillator(FREQUENCY);
  auto o2 = Oscillator(FREQUENCY);
  o1.setEffect(EffectType::SLIDE);

  for (int i = 0; i < Configuration::getBufferSize(); i++) {
    o1.oscillate();
    o2.oscillate();
  }
  assertLessThan("applies effect to frequency", o1.oscillate(), o2.oscillate());

  o1 = Oscillator(FREQUENCY);
  o2 = Oscillator(FREQUENCY);
  o1.setEffect(EffectType::FADEIN);

  for (int i = 0; i < Configuration::getBufferSize(); i++) {
    o1.oscillate();
    o2.oscillate();
  }

  assertLessThan("applies effect to volume", o1.oscillate(), o2.oscillate());
}

auto main() -> int {
  Configuration::setBufferSize(4096);

  suite("Oscillator", []() {
    test("init", init);
    test("pitch", pitch);
    test("effect", effect);
    test("oscillations", oscillations);
  });
}
