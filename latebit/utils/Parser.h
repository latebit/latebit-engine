#pragma once

#include <istream>
using namespace std;

// Reads a stream line by line and returns a string
// accounting for different line endings
auto getLine(istream* file) -> string;

// Gets next line skipping all comment lines in between
auto getNonCommentedLine(istream* stream, char commentChar = '#') -> string;