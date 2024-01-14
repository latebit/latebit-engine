#include "Clock.h"

#include <chrono>

using namespace std;

namespace df {

Clock::Clock() { this->previous_time = getNow(); }

long int Clock::delta() {
  if (!this->previous_time) return -1;
  long int now = getNow();
  long int result = now - this->previous_time;
  this->previous_time = now;

  return result;
}

long int Clock::split() const {
  long int now = getNow();
  return now - this->previous_time;
}

long int Clock::getNow() {
  auto now = chrono::high_resolution_clock::now();
  return chrono::duration_cast<chrono::microseconds>(now.time_since_epoch())
      .count();
}

}  // namespace df
