#include "Math.h"

#include <cstdlib>

auto clamp(float value, float min, float max) -> float {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

auto clamp(int value, int min, int max) -> int {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

auto equals(float a, float b, float tolerance) -> bool {
  return std::abs(b - a) < tolerance;
}
