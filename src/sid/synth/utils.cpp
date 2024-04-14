#include "utils.h"

namespace sid {

auto clamp(float value, float min, float max) -> float {
  return value < min ? min : value > max ? max : value;
}

auto max(int a, int b) -> int { return a > b ? a : b; }

};  // namespace sid
