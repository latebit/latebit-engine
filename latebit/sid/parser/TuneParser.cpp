#include "TuneParser.h"

#include <istream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

#include "../synth/Note.h"
#include "../synth/Tune.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Parser.h"

using namespace std;

namespace sid {
auto getSymbolsFromLine(const string &line,
                        char delimiter = '|') -> vector<string> {
  vector<string> result;
  istringstream ss(line);
  string item = getLine(&ss, delimiter);

  while (!item.empty()) {
    result.push_back(item);
    item = getLine(&ss, delimiter);
  }

  return result;
}

auto TuneParser::fromStream(istream *stream,
                            const ParserOptions *opts) -> unique_ptr<Tune> {
  string version = getLine(stream);
  if (version != "#v0.1#") {
    Log.error(
      "TuneParser::fromStream(): Invalid header. Unsupported version %s",
      version.c_str());
    return nullptr;
  }

  int bpm = getNextNumber(stream);
  if (bpm < 10 || bpm > 400) {
    Log.error("TuneParser::fromStream(): Invalid bpm. %s",
              makeRangeValidationMessage(bpm, 400, 10).c_str());
    return nullptr;
  }

  int ticksPerBeat = getNextNumber(stream);
  if (ticksPerBeat < 1 || ticksPerBeat > opts->maxTicksPerBeat) {
    Log.error(
      "TuneParser::fromStream(): Invalid ticks per beat. %s",
      makeRangeValidationMessage(ticksPerBeat, opts->maxTicksPerBeat).c_str());
    return nullptr;
  }

  int beatsCount = getNextNumber(stream);
  if (beatsCount <= 0 || beatsCount > 64) {
    Log.error(
      "TuneParser::fromStream(): Invalid beats count. %s",
      makeRangeValidationMessage(beatsCount, opts->maxBeatsCount).c_str());
    return nullptr;
  }

  int tracksCount = getNextNumber(stream);
  if (tracksCount <= 0 || tracksCount > opts->maxTracksCount) {
    Log.error(
      "TuneParser::fromStream(): Invalid tracks count. %s",
      makeRangeValidationMessage(tracksCount, opts->maxTracksCount).c_str());
    return nullptr;
  }

  int maxTrackLength = beatsCount * ticksPerBeat;

  // Flags used to stop collection of symbols for a track when the end of track
  // symbol is found
  vector<bool> trackEnded = {};
  vector<unique_ptr<Track>> tracks = {};
  for (int i = 0; i < tracksCount; i++) {
    trackEnded.push_back(false);
    tracks.push_back(make_unique<Track>());
  }

  for (int i = 0; i < maxTrackLength; i++) {
    auto line = getNextNonCommentLine(stream);
    if (line.empty()) {
      Log.error("TuneParser::fromStream(): Unexpected end of file.");
      return nullptr;
    }

    auto symbols = getSymbolsFromLine(line);
    if ((int)symbols.size() != tracksCount) {
      Log.error(
        "TuneParser::fromStream(): Invalid number of symbols in line %d. "
        "Expected %d, got %d",
        i, tracksCount, symbols.size());
      return nullptr;
    }

    for (int j = 0; j < tracksCount; j++) {
      if (trackEnded.at(j)) continue;

      auto symbol = symbols[j];
      if (symbol == END_OF_TRACK_SYMBOL) {
        trackEnded[j] = true;
        continue;
      }

      auto track = tracks.at(j).get();
      if (symbol == REST_SYMBOL) {
        track->push_back(Note::makeRest());
      } else if (symbol == CONTINUE_SYMBOL) {
        if (i == 0) {
          Log.error(
            "TuneParser::fromStream(): First symbol of a track cannot be a "
            "continue symbol");
          return nullptr;
        }

        track->push_back(Note::makeContinue());
      } else {
        auto note = Note::fromSymbol(symbol);

        if (note.getType() == NoteType::Invalid) {
          // error is already printed in the symbol parser
          return nullptr;
        }

        track->push_back(note);
      }
    }
  }

  return make_unique<Tune>(bpm, ticksPerBeat, beatsCount, std::move(tracks));
}

auto TuneParser::fromFile(const string filename,
                          const ParserOptions *opts) -> unique_ptr<Tune> {
  ifstream file(filename);
  if (!file.is_open()) {
    Log.error("TuneParser::fromFile(): Could not open file %s",
              filename.c_str());
    return nullptr;
  }

  return fromStream(&file, opts);
}

auto TuneParser::fromString(string str,
                            const ParserOptions *opts) -> unique_ptr<Tune> {
  istringstream stream(str);
  return fromStream(&stream, opts);
}

auto TuneParser::toString(const Tune &tune) -> string {
  stringstream ss;
  ss << "#v0.1#\n";
  ss << tune.getBpm() << " # tempo\n";
  ss << tune.getTicksPerBeat() << " # ticks per beat\n";
  ss << tune.getBeatsCount() << " # beats count\n";
  ss << tune.getTracksCount() << " # tracks count\n";

  int maxTrackLength = tune.getBeatsCount() * tune.getTicksPerBeat();
  for (int i = 0; i < maxTrackLength; i++) {
    for (int j = 0; j < tune.getTracksCount(); j++) {
      auto &track = tune.getTrack(j);
      if (i >= (int)track->size()) {
        ss << "      ";
      } else {
        ss << track->at(i).getSymbol();
      }

      if (j < tune.getTracksCount() - 1) {
        ss << "|";
      }
    }
    ss << "\n";
  }

  return ss.str();
}

}  // namespace sid
