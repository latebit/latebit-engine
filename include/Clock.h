#pragma once

namespace df {

class Clock {
 private:
  long int previous_time;

 public:
  Clock();

  // Call it in succession to get time since last
  // call in microseconds. First or error return -1
  long int delta();

  // Like delta, but does not override last time
  long int split() const;

  // Returns current time in milliseconds
  static long int getNow();
};

}  // namespace df

