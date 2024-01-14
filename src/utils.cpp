#include "utils.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

auto getTimeString() -> std::string {
  std::time_t now = std::time(nullptr);
  std::tm* lt = std::localtime(&now);

  std::ostringstream stream;
  stream << std::setfill('0') << std::setw(2) << lt->tm_hour << ":"
         << std::setfill('0') << std::setw(2) << lt->tm_min << ":"
         << std::setfill('0') << std::setw(2) << lt->tm_sec;

  return stream.str();
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

  bool xOverlap =
    (bLeft <= aLeft && aLeft <= bRight) || (aLeft <= bLeft && bLeft <= aRight);
  bool yOverlap =
    (bTop <= aTop && aTop <= bBottom) || (aTop <= bTop && bTop <= aBottom);

  return xOverlap && yOverlap;
}
