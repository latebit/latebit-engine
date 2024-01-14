#pragma once

#include <cstdio>
#include <ctime>
#include <string>

#include "Box.h"
#include "Vector.h"

using namespace df;
using namespace std;

auto getTimeString() -> string;
void sleep(long int microseconds);
auto match(Vector p1, Vector p2) -> bool;
auto equals(float a, float b, float tolerance = 0.0001f) -> bool;
auto intersects(Box box1, Box box2) -> bool;