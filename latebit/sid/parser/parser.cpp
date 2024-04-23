#include "parser.h"

#include <istream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

#include "latebit/sid/synth/tune.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Parser.h"
#include "symbol.h"

using namespace std;

namespace sid {
auto getNumber(istream *stream, char commentChar = '#') -> int {
  string line = getNonCommentedLine(stream);
  if (line.empty()) {
    return -1;
  }
  smatch match;
  std::regex numberWithComments("^\\d+\\s*" + string(1, commentChar) + "*.*$");
  if (std::regex_search(line, match, numberWithComments)) {
    // TODO: handle exceptions
    return stoi(match.str());
  } else {
    return -1;
  }
}

auto getSymbolsFromLine(const string &line,
                        char delimiter = '|') -> vector<string> {
  vector<string> result;
  stringstream ss(line);
  string item;

  // TODO: use getLine instead of getline
  while (getline(ss, item, delimiter)) {
    result.push_back(item);
  }

  return result;
}

auto TuneParser::fromString(istream *stream) -> unique_ptr<Tune> {
  string version = getLine(stream);
  if (version != "#v0.1#") {
    Log.error("Invalid header. Unsupported version %s", version.c_str());
    return nullptr;
  }

  int bpm = getNumber(stream);
  if (bpm <= 10 || bpm >= 300) {
    Log.error("Invalid bpm. Expected a number 10-300, got %d", bpm);
    return nullptr;
  }
  int ticksPerBeat = getNumber(stream);
  if (ticksPerBeat <= 0 || ticksPerBeat > 16) {
    Log.error("Invalid ticks per beat. Expected a number 1-16, got %d",
              ticksPerBeat);
    return nullptr;
  }
  int beatsCount = getNumber(stream);
  if (beatsCount <= 0 || beatsCount > 64) {
    Log.error("Invalid beats count. Expected a number 1-64, got %d",
              beatsCount);
    return nullptr;
  }
  int tracksCount = getNumber(stream);
  if (tracksCount <= 0 || tracksCount > 3) {
    Log.error("Invalid tracks count. Expected a number 1-3, got %d",
              tracksCount);
    return nullptr;
  }

  auto t = make_unique<Tune>(tracksCount);
  t->setBpm(bpm);
  t->setTicksPerBeat(ticksPerBeat);
  t->setBeatsCount(beatsCount);

  int maxTrackLength = beatsCount * ticksPerBeat;

  // Flags used to stop collection of symbols for a track when the end of track
  // symbol is found
  vector<bool> trackEnded = {};
  for (int i = 0; i < tracksCount; i++) {
    trackEnded.push_back(false);
  }

  for (int i = 0; i < maxTrackLength; i++) {
    auto line = getNonCommentedLine(stream);
    if (line.empty()) {
      Log.error("Unexpected end of file");
      return nullptr;
    }

    auto symbols = getSymbolsFromLine(line);
    if (symbols.size() != tracksCount) {
      Log.error("Invalid number of symbols in line %d. Expected %d, got %d", i,
                tracksCount, symbols.size());
      return nullptr;
    }

    for (int j = 0; j < tracksCount; j++) {
      if (trackEnded.at(j)) continue;

      auto symbol = symbols[j];
      if (isEndOfTrack(symbol)) {
        trackEnded[j] = true;
        continue;
      }

      auto track = t->getTrack(j);
      if (isRest(symbol)) {
        track->push_back(Note::rest());
      } else if (isContinue(symbol)) {
        if (i == 0) {
          Log.error("First symbol of a track cannot be a continue symbol");
          return nullptr;
        }

        auto last = track->at(track->size() - 1);
        track->push_back(last);
      } else {
        auto note = toNote(symbol);
        if (note.isInvalid()) {
          // error is already printed in the symbol parser
          return nullptr;
        }

        track->push_back(note);
      }
    }
  }

  return t;
}

auto TuneParser::fromFile(const string filename) -> unique_ptr<Tune> {
  ifstream file(filename);
  if (!file.is_open()) {
    Log.error("Could not open file %s", filename.c_str());
    return nullptr;
  }

  return fromString(&file);
}

}  // namespace sid