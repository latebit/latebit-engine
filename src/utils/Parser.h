#pragma once

#include <istream>
using namespace std;

// Reads a stream line by line and returns a string
// accounting for different line endings
auto getLine(istream* file) -> string;