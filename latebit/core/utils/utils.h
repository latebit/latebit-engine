#pragma once

#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"

using namespace lb;
using namespace std;

namespace lb {

// Return true if positions are in the same cell
auto match(Vector p1, Vector p2) -> bool;

// Return true if boxes intersect
auto intersects(Box box1, Box box2) -> bool;

// Return true if container contains content
auto contains(Box container, Box content) -> bool;
}