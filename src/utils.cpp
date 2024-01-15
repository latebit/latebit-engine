#include "utils.h"

#include <cmath>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "Box.h"
#include "Vector.h"
#include "WorldManager.h"

using namespace df;
using namespace std;

auto getTimeString() -> string {
  time_t now = time(nullptr);
  tm* lt = localtime(&now);

  ostringstream stream;
  stream << setfill('0') << setw(2) << lt->tm_hour << ":" << setfill('0')
         << setw(2) << lt->tm_min << ":" << setfill('0') << setw(2)
         << lt->tm_sec;

  return stream.str();
}

void sleep(long int microseconds) {
  this_thread::sleep_for(chrono::microseconds(microseconds));
}

auto match(Vector p1, Vector p2) -> bool {
  return abs(p1.getX() - p2.getX()) <= 1 && abs(p1.getY() - p2.getY()) <= 1;
}

auto equals(float a, float b, float tolerance) -> bool {
  return abs(b - a) < tolerance;
}

auto intersects(Box a, Box b) -> bool {
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

auto worldToView(Vector worldPosition) -> Vector {
  auto viewOrigin = WM.getView().getCorner();
  return worldPosition - viewOrigin;
}

auto clamp(float value, float min, float max) -> float {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}