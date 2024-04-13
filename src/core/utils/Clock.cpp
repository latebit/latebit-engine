#include "Clock.h"

#include <chrono>

using namespace std;

namespace lb {

Clock::Clock() { this->previousTime = getNow(); }

auto Clock::delta() -> long int {
  if (!this->previousTime) return -1;
  long int now = getNow();
  long int result = now - this->previousTime;
  this->previousTime = now;

  return result;
}

auto Clock::split() const -> long int {
  long int now = getNow();
  return now - this->previousTime;
}

auto Clock::getNow() -> long int {
  auto now = chrono::high_resolution_clock::now();
  return chrono::duration_cast<chrono::microseconds>(now.time_since_epoch())
    .count();
}

}  // namespace lb
