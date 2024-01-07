#include <cstdio>
#include <ctime>

#include "Vector.h"

char *getTimeString();
void sleep(long int milliseconds);
bool positionsMatch(df::Vector p1, df::Vector p2);
bool floatEquals(float a, float b, float tolerance = 0.0001f);