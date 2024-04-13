#include "utils.h"

#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "Box.h"
#include "Vector.h"
#include "core/objects/WorldManager.h"

using namespace lb;
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

auto viewToWorld(Vector viewPosition) -> Vector {
  auto viewOrigin = WM.getView().getCorner();
  return viewPosition + viewOrigin;
}

auto clamp(float value, float min, float max) -> float {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

auto clamp(int value, int min, int max) -> int {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

auto contains(Box container, Box content) -> bool {
  float containerLeft = container.getCorner().getX();
  float containerRight = containerLeft + container.getWidth();
  float containerTop = container.getCorner().getY();
  float containerBottom = containerTop + container.getHeight();

  float contentLeft = content.getCorner().getX();
  float contentRight = contentLeft + content.getWidth();
  float contentTop = content.getCorner().getY();
  float contentBottom = contentTop + content.getHeight();

  return containerLeft <= contentLeft && contentRight <= containerRight &&
         containerTop <= contentTop && contentBottom <= containerBottom;
}

auto getLine(ifstream* file) -> string {
  string line;

  getline(*file, line);

  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }

  return line;
}