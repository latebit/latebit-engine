#include "symbol.h"

#include <sstream>

#include "../../../test/lib/test.h"
#include "sid/synth/track.h"

using namespace sid;

void defaultValues() {
  Note n = toNote("--50F0");
  assert("default pitch is rest", n.isRest());

  n = toNote("C-50F0");
  assertEq("default alteration is none", n.getPitch(), 60);

  n = toNote("C#-0F0");
  assertEq("default octave is 4", n.getPitch(), 49);

  n = toNote("C#5-F0");
  assertEq("default wave is TRIANGLE", n.getWave(), TRIANGLE);

  n = toNote("C#51-0");
  assertEq("default volume is 8", n.getVolume(), 0.53333);

  n = toNote("C#519-");
  assertEq("default volume is NONE", n.getEffect(), NONE);
}

void alterations() {
  Note n = toNote("C#51F0");
  assertEq("parses sharp", n.getPitch(), 61);

  n = toNote("Eb51F0");
  assertEq("parses flat", n.getPitch(), 63);

  n = toNote("B#51F0");
  assertEq("parses sharp (boundary)", n.getPitch(), 72);

  n = toNote("Cb51F0");
  assertEq("parses flat (boundary)", n.getPitch(), 59);
}

void octaves() {
  for (int i = 0; i < 8; i++) {
    stringstream symbolStream;
    symbolStream << "C#" << i << "0F0";
    string symbol = symbolStream.str();

    stringstream nameStream;
    nameStream << "parses octave " << i;
    string name = nameStream.str();

    Note n = toNote(symbol);
    assertEq(name, n.getPitch(), 1 + (12 * i));
  }
}

void waves() {
  Note n = toNote("C#50F0");
  assertEq("parses TRIANGLE", n.getWave(), TRIANGLE);

  n = toNote("C#51F0");
  assertEq("parses SAWTOOTH", n.getWave(), SAWTOOTH);

  n = toNote("C#52F0");
  assertEq("parses SQUARE", n.getWave(), SQUARE);

  n = toNote("C#53F0");
  assertEq("parses NOISE", n.getWave(), NOISE);
}

void volume() {
  Note n = toNote("C#51F0");
  assertEq("parses volume 15", n.getVolume(), 1.0);

  n = toNote("C#51E1");
  assertEq("parses volume 14", n.getVolume(), 0.933333);

  n = toNote("C#5192");
  assertEq("parses volume 9", n.getVolume(), 0.6);
}

void effects() {
  Note n = toNote("C#51F0");
  assertEq("parses effect NONE", n.getEffect(), NONE);
  n = toNote("C#51F1");
  assertEq("parses effect DROP", n.getEffect(), DROP);
  n = toNote("C#51F2");
  assertEq("parses effect SLIDE", n.getEffect(), SLIDE);
  n = toNote("C#51F3");
  assertEq("parses effect FADEIN", n.getEffect(), FADEIN);
  n = toNote("C#51F4");
  assertEq("parses effect FADEOUT", n.getEffect(), FADEOUT);
}

void invalid() {
  Note n = toNote("");
  assert("empty string yields invalid", n.isInvalid());

  n = toNote("C#51F");
  assert("incomplete string yields invalid", n.isInvalid());

  n = toNote("C#51F5");
  assert("invalid effect yields invalid", n.isInvalid());

  n = toNote("C#51E5");
  assert("invalid volume yields invalid", n.isInvalid());

  n = toNote("C#51D5");
  assert("invalid wave yields invalid", n.isInvalid());

  n = toNote("C#51C5");
  assert("invalid pitch yields invalid", n.isInvalid());

  n = toNote("C#51B5");
  assert("invalid alteration yields invalid", n.isInvalid());
}

auto main() -> int {
  Note n = toNote("C#51F1");
  assertEq("correct pitch", n.getPitch(), 61);
  assertEq("correct wave", n.getWave(), WaveType::SAWTOOTH);
  assertEq("correct volume", n.getVolume(), 1.0);
  assertEq("correct effect", n.getEffect(), EffectType::DROP);

  test("default values", defaultValues);
  test("alterations", alterations);
  test("octaves", octaves);
  test("waves", waves);
  test("volume", volume);
  test("effects", effects);
  test("invalid", invalid);

  return report();
}