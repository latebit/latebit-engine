#include <array>

#include "Configuration.h"
#include "Effect.h"
#include "test/lib/test.h"

using namespace sid;

void frequency() {
  Effect e = DROP_EFFECT;
  array<float, 2> result = {};
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertLessThan("reduces the frequency", result[0], 100);
  assertLessThan("reduces the frequency", result[1], result[0]);

  e = SLIDE_EFFECT;
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertLessThan("increases the frequency", 100, result[0]);
  assertLessThan("increases the frequency", result[0], result[1]);

  e = FADEIN_EFFECT;
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertEq("doesn't change the frequency", result[0], 100.0);
  assertEq("doesn't change the frequency", result[1], result[0]);

  e = FADEOUT_EFFECT;
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertEq("doesn't change the frequency", result[0], 100.0);
  assertEq("doesn't change the frequency", result[1], result[0]);

  e = NONE_EFFECT;
  e.processFrequency(100);
  result[0] = e.processFrequency(100);
  result[1] = e.processFrequency(100);
  assertEq("doesn't change the frequency", result[0], 100.0);
  assertEq("doesn't change the frequency", result[1], result[0]);
}

void volume() {
  Effect e = FADEIN_EFFECT;
  array<float, 3> result = {};

  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertLessThan("increases the volume", result[0], result[1]);
  assertLessThan("increases the volume", result[1], result[2]);

  e = FADEOUT_EFFECT;
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertLessThan("decreases the volume", result[1], result[0]);
  assertLessThan("decreases the volume", result[2], result[1]);

  e = DROP_EFFECT;
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertEq("doesn't change the volume", result[1], 0.5);
  assertEq("doesn't change the volume", result[2], result[1]);

  e = NONE_EFFECT;
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertEq("doesn't change the volume", result[1], 0.5);
  assertEq("doesn't change the volume", result[2], result[1]);

  e = SLIDE_EFFECT;
  result[0] = e.processVolume(0.5);
  result[1] = e.processVolume(0.5);
  result[2] = e.processVolume(0.5);
  assertEq("doesn't change the volume", result[1], 0.5);
  assertEq("doesn't change the volume", result[2], result[1]);
}

auto main() -> int {
  Configuration::setBufferSize(4096);

  suite("Effect", []() {
    test("frequency", frequency);
    test("volume", volume);
  });
}
