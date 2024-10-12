#include "utils.h"

#include <cmath>
#include <ctime>

#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"

using namespace lb;
using namespace std;

namespace lb {
auto match(Vector p1, Vector p2) -> bool {
  return abs(p1.getX() - p2.getX()) <= 1 && abs(p1.getY() - p2.getY()) <= 1;
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
}