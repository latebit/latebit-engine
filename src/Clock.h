#ifndef __CLOCK_H__
#define __CLOCK_H__

namespace df {

class Clock {
 private:
  long int m_previous_time;

  // Returns current time in milliseconds
  static long int getNow();

 public:
  Clock();

  // Call it in succession to get time since last
  // call in microseconds. First or error return -1
  long int delta();

  // Like delta, but does not override last time
  long int split() const;
};

}  // namespace df

#endif