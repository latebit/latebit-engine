#pragma once

#include <istream>
#include <memory>

#include "latebit/sid/synth/Tune.h"

using namespace std;

namespace sid {

class TuneParser {
 private:
  static auto fromStream(istream *str) -> unique_ptr<Tune>;

 public:
  static auto fromFile(const string filename) -> unique_ptr<Tune>;
  static auto fromString(string str) -> unique_ptr<Tune>;
  static auto toString(const Tune &tune) -> string;
};

}  // namespace sid
