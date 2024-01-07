#include "utils.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <thread>

char *getTimeString() {
  static char time_str[30];

  time_t now;
  time(&now);
  struct tm *p_localtime = localtime(&now);

  sprintf(time_str, "%02d:%02d:%02d", p_localtime->tm_hour, p_localtime->tm_min,
          p_localtime->tm_sec);

  return time_str;
}

void sleep(long int microseconds) {
  std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

bool positionsMatch(df::Vector p1, df::Vector p2) {
  return std::abs(p1.getX() - p2.getX()) <= 1 &&
         std::abs(p1.getY() - p2.getY()) <= 1;
}

bool floatEquals(float a, float b, float tolerance) {
  return std::abs(b - a) < tolerance;
}