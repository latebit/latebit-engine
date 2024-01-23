#pragma once

namespace df {

class Clock {
 private:
  long int previous_time;

 public:
  Clock();

  // Call it in succession to get time since last
  // call in microseconds. First or error return -1
  auto delta() -> long int;

  // Like delta, but does not override last time
  [[nodiscard]] auto split() const -> long int;

  // Returns current time in milliseconds
  static auto getNow() -> long int;
};

}  // namespace df
