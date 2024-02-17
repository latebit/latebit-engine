#pragma once

#include <cstdio>
#include <ctime>
#include <string>

#include "Box.h"
#include "Vector.h"

using namespace lb;
using namespace std;

// Return current time as string
auto getTimeString() -> string;

// Sleep for given number of microseconds
void sleep(long int microseconds);

// Return true if positions are in the same cell
auto match(Vector p1, Vector p2) -> bool;

// Return true if floats are equal within tolerance
auto equals(float a, float b, float tolerance = 0.0001f) -> bool;

// Return true if boxes intersect
auto intersects(Box box1, Box box2) -> bool;

// Convert world coordinates to view coordinates
auto worldToView(Vector worldPosition) -> Vector;

// Convert view coordinates to world coordinates
auto viewToWorld(Vector viewPosition) -> Vector;

// Clamp value between min and max
auto clamp(float value, float min, float max) -> float;

// Return true if container contains content
auto contains(Box container, Box content) -> bool;