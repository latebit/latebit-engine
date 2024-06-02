#pragma once

#include <istream>
#include <memory>

#include "latebit/sid/synth/Tune.h"

using namespace std;

namespace sid {

// The ParserOptions struct contains the options used by the TuneParser class to
// define boundaries for the parsing of a Tune.
struct ParserOptions {
  // Maximum number of tracks in a tune.
  uint8_t maxTracksCount;
  // Maximum number of beats in a tune.
  uint8_t maxBeatsCount;
  // Maximum number of ticks per beat in the tune.
  uint8_t maxTicksPerBeat;
};

// The TuneParser class is responsible for parsing a Tune from files and strings
// and converting a Tune back to a string.
// The format used for the Tune is the SID format and you can read more about it
// [here](https://github.com/latebit/latebit-engine/blob/master/docs/specs/sid-v0.md).
// The TuneParser class is stateless and thread-safe.
class TuneParser {
 private:
  // Parses a Tune from a stream. Returns nullptr if the stream is invalid.
  static auto fromStream(istream *str,
                         const ParserOptions *opts) -> unique_ptr<Tune>;

 public:
  // Parses a Tune from a file. It's a wrapper around TuneParser::fromStream.
  // Returns nullptr if the file is invalid.
  static auto fromFile(const string filename,
                       const ParserOptions *opts) -> unique_ptr<Tune>;
  // Parses a Tune from a string. It's a wrapper around TuneParser::fromStream.
  // Returns nullptr if the string is invalid.
  static auto fromString(string str,
                         const ParserOptions *opts) -> unique_ptr<Tune>;
  // Converts a Tune to a string.
  static auto toString(const Tune &tune) -> string;
};

}  // namespace sid
