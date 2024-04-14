#include "utils.h"

namespace sid {

auto clamp(float value, float min, float max) -> float {
  return value < min ? min : value > max ? max : value;
}

};  // namespace sid
