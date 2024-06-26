#include "TuneParser.h"

#include <memory>

#include "test/lib/test.h"

using namespace sid;

const ParserOptions DEFAULT_PARSER_OPTIONS = {
  .maxTracksCount = 3, .maxBeatsCount = 64, .maxTicksPerBeat = 16};

void simplest() {
  string str =
    "#v0.1#\n90\n1\n2\n3\nC-4---|D-4---|------\nC-5---|......|------\n";
  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);
  assertEq("parses bpm", t->getBpm(), 90);
  assertEq("parses ticks per beat", t->getTicksPerBeat(), 1);
  assertEq("parses beats", t->getBeatsCount(), 2);
  assertEq("parses tracks count", t->getTracksCount(), 3);

  auto& track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 2);
  assert("reads note correctly", track->at(0) == Note::fromSymbol("C-4---"));
  assert("reads note correctly", track->at(1) == Note::fromSymbol("C-5---"));

  auto& track1 = t->getTrack(1);
  assertEq("track 1 has correct track length", track1->size(), 2);
  assert("reads note correctly", track1->at(0) == Note::fromSymbol("D-4---"));
  assert("reads note correctly", track1->at(1).getType() == NoteType::CONTINUE);

  auto& track2 = t->getTrack(2);
  assertEq("track 2 has correct track length", track2->size(), 2);
  assert("reads note correctly", track2->at(0).getType() == NoteType::REST);
  assert("reads note correctly", track2->at(1).getType() == NoteType::REST);
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
  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);
  assertEq("parses bpm", t->getBpm(), 90);
  assertEq("parses ticks per beat", t->getTicksPerBeat(), 1);
  assertEq("parses beats", t->getBeatsCount(), 2);
  assertEq("parses tracks count", t->getTracksCount(), 3);

  auto& track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 2);
  assert("reads note correctly", track->at(0) == Note::fromSymbol("C-4---"));
  assert("reads note correctly", track->at(1) == Note::fromSymbol("C-5---"));

  auto& track1 = t->getTrack(1);
  assertEq("track 1 has correct track length", track1->size(), 2);
  assert("reads note correctly", track1->at(0) == Note::fromSymbol("D-4---"));
  assert("reads note correctly", track1->at(1).getType() == NoteType::CONTINUE);

  auto& track2 = t->getTrack(2);
  assertEq("track 2 has correct track length", track2->size(), 2);
  assert("reads note correctly", track2->at(0).getType() == NoteType::REST);
  assert("reads note correctly", track2->at(1).getType() == NoteType::REST);
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
  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);
  assertEq("has correct track length (0)", t->getTrack(0)->size(), 3);
  assertEq("has correct track length (1)", t->getTrack(1)->size(), 1);
  assertEq("has correct track length (2)", t->getTrack(2)->size(), 2);
  assert("reads correct note (0,0)",
         t->getTrack(0)->at(0) == Note::fromSymbol("C-4---"));
  assert("reads correct note (0,1)",
         t->getTrack(0)->at(1) == Note::fromSymbol("G-4---"));
  assert("reads correct note (0,2)",
         t->getTrack(0)->at(2) == Note::fromSymbol("E-4---"));
  assert("reads correct note (1,0)",
         t->getTrack(1)->at(0) == Note::fromSymbol("C-4---"));
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

  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);

  auto& track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 3);
  assert("reads note correctly", track->at(0) == Note::fromSymbol("C-4---"));
  assert("reads note correctly", track->at(1) == Note::makeRest());
  assert("reads note correctly", track->at(2) == Note::fromSymbol("E-4---"));
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

  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);

  auto& track = t->getTrack(0);
  assertEq("track 0 has correct track length", track->size(), 3);
  assert("has correct first note", track->at(0) == Note::fromSymbol("C-4---"));
  assert("keeps correct note", track->at(1).getType() == NoteType::CONTINUE);
  assert("stops note", track->at(2).getType() == NoteType::REST);

  str =
    "#v0.1#\n"
    "90\n"
    "1\n"
    "2\n"
    "1\n"
    "------\n"
    "......\n";

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  auto& track1 = t->getTrack(0);
  assertEq("track 0 has correct track length", track1->size(), 2);
  assert("registers pause", track1->at(0).getType() == NoteType::REST);
  assert("keeps the pause", track1->at(1).getType() == NoteType::CONTINUE);
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

  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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
  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("does not parse the tune with invalid bpm (too small)", t == nullptr);

  str =
    "#v0.1#\n"
    "401\n"
    "1\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

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

  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("does not parse the tune with invalid tracks count (too big)",
         t == nullptr);

  str =
    "#v0.1#\n"
    "10\n"
    "1\n"
    "1\n"
    "1\n"
    "C-4---\n";
  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);
  assertEq("parses bpm", t->getBpm(), 10);
  assertEq("parses ticks per beat", t->getTicksPerBeat(), 1);
  assertEq("parses beats", t->getBeatsCount(), 1);
  assertEq("parses tracks count", t->getTracksCount(), 1);

  str = "#v0.1#\n400\n1\n1\n1\nC-4---\n";
  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);
  assert("parses tune", t != nullptr);

  str =
    "#v0.1#\n400\n16\n1\n1\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-"
    "4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4-"
    "--\n";
  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);
  assert("parses tune", t != nullptr);

  str =
    "#v0.1#\n400\n1\n64\n1\n"
    "C-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-"
    "4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\n"
    "C-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-"
    "4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\n"
    "C-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-"
    "4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\n"
    "C-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-"
    "4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\nC-4---\n";
  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);
  assert("parses tune", t != nullptr);

  str = "#v0.1#\n400\n1\n1\n3\nC-4---|C-4---|C-4---\n";
  t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);
  assert("parses tune", t != nullptr);
}

void consistency() {
  string str =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "3 # beat count\n"
    "1 # track count\n"
    "C-4---\n"
    "......\n"
    "------\n";

  unique_ptr<Tune> t = TuneParser::fromString(str, &DEFAULT_PARSER_OPTIONS);

  assert("parses tune", t != nullptr);
  string out = TuneParser::toString(*t);
  assertEq("parses back to string", out, str);

  string strWithComments =
    "#v0.1#\n"
    "90\n"
    "1 # with comment\n"
    "3\n"
    "1\n"
    "C-4---\n"
    "......\n"
    "------\n";

  t = TuneParser::fromString(strWithComments, &DEFAULT_PARSER_OPTIONS);
  assert("parses tune", t != nullptr);
  out = TuneParser::toString(*t);
  assertEq("replaces comments", out, str);
}

void correctOutput() {
  vector<unique_ptr<Track>> tracks;
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("D-4---"));
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("E-4---"));
  tracks.back()->push_back(Note::fromSymbol("F-4---"));
  Tune tune(90, 1, 2, std::move(tracks));

  string expected =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "2 # beat count\n"
    "2 # track count\n"
    "C-4---|E-4---\n"
    "D-4---|F-4---\n";

  assertEq("returns expected output", TuneParser::toString(tune), expected);
}

void emptyTrack() {
  vector<unique_ptr<Track>> tracks;
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("D-4---"));
  tracks.push_back(make_unique<Track>());
  Tune tune(90, 1, 2, std::move(tracks));

  string expected =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "2 # beat count\n"
    "2 # track count\n"
    "C-4---|      \n"
    "D-4---|      \n";

  assertEq("returns expected output", TuneParser::toString(tune), expected);
}

void unequalLengths() {
  vector<unique_ptr<Track>> tracks;
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::fromSymbol("D-4---"));
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("E-4---"));
  Tune tune(90, 1, 2, std::move(tracks));

  string expected =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "2 # beat count\n"
    "2 # track count\n"
    "C-4---|E-4---\n"
    "D-4---|      \n";

  assertEq("returns expected output", TuneParser::toString(tune), expected);
}

void specialSymbols() {
  vector<unique_ptr<Track>> tracks;
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::fromSymbol("C-4---"));
  tracks.back()->push_back(Note::makeContinue());
  tracks.push_back(make_unique<Track>());
  tracks.back()->push_back(Note::makeRest());
  Tune tune(90, 1, 2, std::move(tracks));

  string expected =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "2 # beat count\n"
    "2 # track count\n"
    "C-4---|------\n"
    "......|      \n";

  assertEq("returns expected output", TuneParser::toString(tune), expected);
}

void withDifferentOptions() {
  ParserOptions opts = {
    .maxTracksCount = 1, .maxBeatsCount = 2, .maxTicksPerBeat = 1};

  string valid =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "2 # beat count\n"
    "1 # track count\n"
    "C-4---\n"
    "D-4---\n";

  assert("does not fail with valid input",
         TuneParser::fromString(valid, &opts) != nullptr);

  string validButNotCompliant =
    "#v0.1#\n"
    "90 # tempo\n"
    "1 # ticks per beat\n"
    "2 # beat count\n"
    "2 # track count\n"
    "C-4---|C-4---\n"
    "D-4---|C-4---\n";

  assert("fails with non compliant input",
         TuneParser::fromString(validButNotCompliant, &opts) == nullptr);
}

auto main() -> int {
  suite("fromString", []() {
    test("with simplest input", simplest);
    test("with comments", withComments);
    test("with different lengths", differentLengths);
    test("with rests", rests);
    test("with continues", continues);
    test("with broken header", brokenHeader);
    test("with valid header", header);
    test("with parsing options", withDifferentOptions);
  });

  suite("toString", []() {
    test("returns correct output", correctOutput);
    test("produces consistent after conversion", consistency);
    test("handles empty tracks", emptyTrack);
    test("handles unequal lengths", unequalLengths);
    test("handles special symbols", specialSymbols);
  });

  return report();
}
