#pragma once

#include <istream>
#include <memory>

#include "latebit/sid/synth/tune.h"

using namespace std;

namespace sid {

class TuneParser {
 public:
  static auto fromFile(const string filename) -> unique_ptr<Tune>;
  static auto fromString(istream *str) -> unique_ptr<Tune>;
};

}  // namespace sid
