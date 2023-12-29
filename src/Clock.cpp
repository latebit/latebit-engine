#include "Clock.h"

#include <chrono>

using namespace std;

namespace df {

Clock::Clock() { m_previous_time = getNow(); }

long int Clock::delta() {
  if (!m_previous_time) return -1;
  long int now = getNow();
  long int result = now - m_previous_time;
  m_previous_time = now;

  return result;
}

long int Clock::split() const {
  long int now = getNow();
  return now - m_previous_time;
}

long int Clock::getNow() {
  auto now = chrono::high_resolution_clock::now();
  return chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch())
      .count();
}

}  // namespace df
