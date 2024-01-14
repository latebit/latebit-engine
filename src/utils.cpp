#include "utils.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <thread>

auto getTimeString() -> char * {
  static char time_str[30];

  time_t now;
  time(&now);
  struct tm *lt = localtime(&now);

  sprintf(time_str, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);

  return time_str;
}

void sleep(long int microseconds) {
  std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

auto match(df::Vector p1, df::Vector p2) -> bool {
  return std::abs(p1.getX() - p2.getX()) <= 1 &&
         std::abs(p1.getY() - p2.getY()) <= 1;
}

auto equals(float a, float b, float tolerance) -> bool {
  return std::abs(b - a) < tolerance;
}

auto intersects(df::Box a, df::Box b) -> bool {
  float aLeft = a.getCorner().getX();
  float aRight = aLeft + a.getWidth();
  float aTop = a.getCorner().getY();
  float aBottom = aTop + a.getHeight();

  float bLeft = b.getCorner().getX();
  float bRight = bLeft + b.getWidth();
  float bTop = b.getCorner().getY();
  float bBottom = bTop + b.getHeight();

  bool xOverlap = (bLeft <= aLeft && aLeft <= bRight) ||
                  (aLeft <= bLeft && bLeft <= aRight);
  bool yOverlap =
      (bTop <= aTop && aTop <= bBottom) || (aTop <= bTop && bTop <= aBottom);

  return xOverlap && yOverlap;
}
