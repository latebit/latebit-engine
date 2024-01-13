#include <cstdio>
#include <ctime>

#include "Box.h"
#include "Vector.h"

char *getTimeString();
void sleep(long int microseconds);
bool match(df::Vector p1, df::Vector p2);
bool equals(float a, float b, float tolerance = 0.0001f);
bool intersects(df::Box box1, df::Box box2);