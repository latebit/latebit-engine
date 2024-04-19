#include "parser.h"

#include <memory>
#include <sstream>

#include "../../../test/lib/test.h"
#include "sid/synth/oscillator.h"

using namespace sid;

void simplest() {
  string str =
    "#v0.1#\n90\n1\n2\n3\nC-4---|D-4---|------\nC-5---|......|------\n";
  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("parses tune", t != nullptr);
  assertEq("parses bpm", t->getBpm(), 90);
  assertEq("parses ticks per beat", t->getTicksPerBeat(), 1);
  assertEq("parses beats", t->getBeatsCount(), 2);
  assertEq("parses tracks count", t->getTracksCount(), 3);

  auto track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 2);
  assert("reads note correctly",
         track->at(0).isEqual(Note(48, 8, TRIANGLE, NONE)));
  assert("reads note correctly",
         track->at(1).isEqual(Note(60, 8, TRIANGLE, NONE)));

  track = t->getTrack(1);
  assertEq("track 1 has correct track length", track->size(), 2);
  assert("reads note correctly",
         track->at(0).isEqual(Note(50, 8, TRIANGLE, NONE)));
  assert("reads note correctly",
         track->at(1).isEqual(Note(50, 8, TRIANGLE, NONE)));

  track = t->getTrack(2);
  assertEq("track 2 has correct track length", track->size(), 2);
  assert("reads note correctly", track->at(0).isEqual(Note::rest()));
  assert("reads note correctly", track->at(1).isEqual(Note::rest()));
}

void withComments() {
  string str =
    "#v0.1#\n"
    "90 # BPM\n"
    "# Ticks per beat\n"
    "1\n"
    "2\n"
    "3\n"
    "C-4---|D-4---|------\n"
    "C-5---|......|------\n";
  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("parses tune", t != nullptr);
  assertEq("parses bpm", t->getBpm(), 90);
  assertEq("parses ticks per beat", t->getTicksPerBeat(), 1);
  assertEq("parses beats", t->getBeatsCount(), 2);
  assertEq("parses tracks count", t->getTracksCount(), 3);

  auto track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 2);
  assert("reads note correctly",
         track->at(0).isEqual(Note(48, 8, TRIANGLE, NONE)));
  assert("reads note correctly",
         track->at(1).isEqual(Note(60, 8, TRIANGLE, NONE)));

  track = t->getTrack(1);
  assertEq("track 1 has correct track length", track->size(), 2);
  assert("reads note correctly",
         track->at(0).isEqual(Note(50, 8, TRIANGLE, NONE)));
  assert("reads note correctly",
         track->at(1).isEqual(Note(50, 8, TRIANGLE, NONE)));

  track = t->getTrack(2);
  assertEq("track 2 has correct track length", track->size(), 2);
  assert("reads note correctly", track->at(0).isEqual(Note::rest()));
  assert("reads note correctly", track->at(1).isEqual(Note::rest()));
}

void differentLengths() {
  string str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "3\n"
    "3\n"
    "C-4---|C-4---|B-4---\n"
    "G-4---|      |C-4---\n"
    "E-4---|      |      \n";

  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("parses tune", t != nullptr);
  assertEq("has correct track length (0)", t->getTrack(0)->size(), 3);
  assertEq("has correct track length (1)", t->getTrack(1)->size(), 1);
  assertEq("has correct track length (2)", t->getTrack(2)->size(), 2);
  assert("reads correct note (0,0)",
         t->getTrack(0)->at(0).isEqual(Note(48, 8, TRIANGLE, NONE)));
  assert("reads correct note (0,1)",
         t->getTrack(0)->at(1).isEqual(Note(55, 8, TRIANGLE, NONE)));
  assert("reads correct note (0,2)",
         t->getTrack(0)->at(2).isEqual(Note(52, 8, TRIANGLE, NONE)));
  assert("reads correct note (1,0)",
         t->getTrack(1)->at(0).isEqual(Note(48, 8, TRIANGLE, NONE)));
}

void rests() {
  string str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "------\n"
    "E-4---\n";

  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("parses tune", t != nullptr);

  auto track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 3);
  assert("reads note correctly",
         track->at(0).isEqual(Note(48, 8, TRIANGLE, NONE)));

  assert("reads note correctly", track->at(1).isEqual(Note::rest()));

  assert("reads note correctly",
         track->at(2).isEqual(Note(52, 8, TRIANGLE, NONE)));
}

void continues() {
  string str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("parses tune", t != nullptr);

  auto track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 3);
  assert("has correct first note",
         track->at(0).isEqual(Note(48, 8, TRIANGLE, NONE)));
  assert("keeps correct note",
         track->at(1).isEqual(Note(48, 8, TRIANGLE, NONE)));
  assert("stops note", track->at(2).isEqual(Note::rest()));

  str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "2\n"
    "1\n"
    "------\n"
    "......\n";

  istringstream stream2(str);
  t = TuneParser::fromString(&stream2);

  track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 2);
  assert("registers pause", track->at(0).isEqual(Note::rest()));
  assert("keeps the pause", track->at(1).isEqual(Note::rest()));
}

void brokenHeader() {
  string str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "3\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("does not parse with missing data", t == nullptr);

  str =
    "#v0.1ff#\n"
    "90\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream2(str);
  t = TuneParser::fromString(&stream2);

  assert("does not parse with invalid version", t == nullptr);

  str =
    "#v0.1#\n"
    "false\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "......\n";

  istringstream stream3(str);
  t = TuneParser::fromString(&stream3);

  assert("does not parse with invalid numbers", t == nullptr);
}

void header() {
  string str =
    "#v0.2#\n"
    "90\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream(str);
  unique_ptr<Tune> t = TuneParser::fromString(&stream);

  assert("does not parse the tune with invalid version", t == nullptr);

  str =
    "#v0.1#\n"
    "0\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream2(str);
  t = TuneParser::fromString(&stream2);

  assert("does not parse the tune with invalid bpm (too small)", t == nullptr);

  str =
    "#v0.1#\n"
    "400\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream3(str);
  t = TuneParser::fromString(&stream3);

  assert("does not parse the tune with invalid bpm (too big)", t == nullptr);

  str =
    "#v0.1#\n"
    "90\n"
    "0\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream4(str);
  t = TuneParser::fromString(&stream4);

  assert("does not parse the tune with invalid ticks per beat (too small)",
         t == nullptr);

  str =
    "#v0.1#\n"
    "90\n"
    "17\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream5(str);
  t = TuneParser::fromString(&stream5);

  assert("does not parse the tune with invalid ticks per beat (too big)",
         t == nullptr);

  str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "0\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream6(str);
  t = TuneParser::fromString(&stream6);

  assert("does not parse the tune with invalid beats count (too small)",
         t == nullptr);

  str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "65\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream7(str);
  t = TuneParser::fromString(&stream7);

  assert("does not parse the tune with invalid beats count (too big)",
         t == nullptr);

  str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "3\n"
    "0\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream8(str);
  t = TuneParser::fromString(&stream8);

  assert("does not parse the tune with invalid tracks count (too small)",
         t == nullptr);

  str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "3\n"
    "4\n"
    "C-4---\n"
    "......\n"
    "------\n";

  istringstream stream9(str);
  t = TuneParser::fromString(&stream9);

  assert("does not parse the tune with invalid tracks count (too big)",
         t == nullptr);
}

auto main() -> int {
  test("simplest", simplest);
  test("withComments", withComments);
  test("differentLengths", differentLengths);
  test("rests", rests);
  test("continues", continues);
  test("brokenHeader", brokenHeader);
  test("header", header);

  return report();
}
