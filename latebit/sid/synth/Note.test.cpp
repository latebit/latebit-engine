#include "Note.h"

#include <sstream>

#include "test/lib/test.h"

using namespace sid;

const Note REST = Note::makeRest();
const Note INVALID = Note::makeInvalid();
const Note CONTINUE = Note::makeContinue();

void defaultValues() {
  Note n = Note::fromSymbol("--50F0");
  assert("default pitch is rest", n.getType() == NoteType::Rest);

  n = Note::fromSymbol("C-50F0");
  assertEq("default alteration is none", n.getPitch(), 60);

  n = Note::fromSymbol("C#-0F0");
  assertEq("default octave is 4", n.getPitch(), 49);

  n = Note::fromSymbol("C#5-F0");
  assertEq("default wave is TRIANGLE", n.getWave(), TRIANGLE);

  n = Note::fromSymbol("C#51-0");
  assertEq("default volume is 8", n.getVolume(), 0.53333);

  n = Note::fromSymbol("C#519-");
  assertEq("default volume is NONE", n.getEffect(), NONE);
}

void alterations() {
  Note n = Note::fromSymbol("C#51F0");
  assertEq("parses sharp", n.getPitch(), 61);

  n = Note::fromSymbol("Eb51F0");
  assertEq("parses flat", n.getPitch(), 63);

  n = Note::fromSymbol("B#51F0");
  assertEq("parses sharp (boundary)", n.getPitch(), 72);

  n = Note::fromSymbol("Cb51F0");
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

    Note n = Note::fromSymbol(symbol);
    assertEq(name, n.getPitch(), 1 + (12 * i));
  }
}

void waves() {
  Note n = Note::fromSymbol("C#50F0");
  assertEq("parses TRIANGLE", n.getWave(), TRIANGLE);

  n = Note::fromSymbol("C#51F0");
  assertEq("parses SAWTOOTH", n.getWave(), SAWTOOTH);

  n = Note::fromSymbol("C#52F0");
  assertEq("parses SQUARE", n.getWave(), SQUARE);

  n = Note::fromSymbol("C#53F0");
  assertEq("parses NOISE", n.getWave(), NOISE);
}

void volume() {
  Note n = Note::fromSymbol("C#51F0");
  assertEq("parses volume 15", n.getVolume(), 1.0);

  n = Note::fromSymbol("C#51E1");
  assertEq("parses volume 14", n.getVolume(), 0.933333);

  n = Note::fromSymbol("C#5192");
  assertEq("parses volume 9", n.getVolume(), 0.6);
}

void effects() {
  Note n = Note::fromSymbol("C#51F0");
  assertEq("parses effect NONE", n.getEffect(), NONE);
  n = Note::fromSymbol("C#51F1");
  assertEq("parses effect DROP", n.getEffect(), DROP);
  n = Note::fromSymbol("C#51F2");
  assertEq("parses effect SLIDE", n.getEffect(), SLIDE);
  n = Note::fromSymbol("C#51F3");
  assertEq("parses effect FADEIN", n.getEffect(), FADEIN);
  n = Note::fromSymbol("C#51F4");
  assertEq("parses effect FADEOUT", n.getEffect(), FADEOUT);
}

void invalid() {
  Note n = Note::fromSymbol("");
  assert("empty string yields invalid", n.getType() == NoteType::Invalid);

  n = Note::fromSymbol("C#51F");
  assert("incomplete string yields invalid", n.getType() == NoteType::Invalid);

  n = Note::fromSymbol("C#51F5");
  assert("invalid effect yields invalid", n.getType() == NoteType::Invalid);

  n = Note::fromSymbol("C#51E5");
  assert("invalid volume yields invalid", n.getType() == NoteType::Invalid);

  n = Note::fromSymbol("C#51D5");
  assert("invalid wave yields invalid", n.getType() == NoteType::Invalid);

  n = Note::fromSymbol("C#51C5");
  assert("invalid pitch yields invalid", n.getType() == NoteType::Invalid);

  n = Note::fromSymbol("C#51B5");
  assert("invalid alteration yields invalid", n.getType() == NoteType::Invalid);
}

void equals() {
  Note n = Note::fromSymbol("C-4---");
  Note m = Note::fromSymbol("C-4---");
  Note r = Note::makeRest();
  Note i = Note::makeInvalid();
  Note c = Note::makeContinue();

  assert("deep compares different notes", n == m);
  assert("detects rest", r == Note::makeRest());
  assert("detects invalid", i == Note::makeInvalid());
  assert("detects continue", c == Note::makeContinue());
  assert("rest is not invalid", r != i);
  assert("rest is not standard", r != n);
  assert("rest is not continue", r != c);
  assert("invalid is not standard", i != n);
  assert("invalid is not continue", i != c);
  assert("continue is not standard", c != n);

  assert("has correct type (standard)", n.getType() == NoteType::Standard);
  assert("has correct type (rest)", r.getType() == NoteType::Rest);
  assert("has correct type (invalid)", i.getType() == NoteType::Invalid);
  assert("has correct type (continue)", c.getType() == NoteType::Continue);
}

auto main() -> int {
  test("==", equals);

  Note n = Note::fromSymbol("C#51F1");
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
