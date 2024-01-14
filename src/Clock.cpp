#include "Clock.h"

#include <chrono>

using namespace std;

namespace df {

Clock::Clock() { this->previous_time = getNow(); }

auto Clock::delta() -> long int {
  if (!this->previous_time) return -1;
  long int now = getNow();
  long int result = now - this->previous_time;
  this->previous_time = now;

  return result;
}

auto Clock::split() const -> long int {
  long int now = getNow();
  return now - this->previous_time;
}

auto Clock::getNow() -> long int {
  auto now = chrono::high_resolution_clock::now();
  return chrono::duration_cast<chrono::microseconds>(now.time_since_epoch())
    .count();
}

}  // namespace df
