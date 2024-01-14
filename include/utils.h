#pragma once

#include <cstdio>
#include <ctime>

#include "Box.h"
#include "Vector.h"

auto getTimeString() -> char *;
void sleep(long int microseconds);
auto match(df::Vector p1, df::Vector p2) -> bool;
auto equals(float a, float b, float tolerance = 0.0001f) -> bool;
auto intersects(df::Box box1, df::Box box2) -> bool;