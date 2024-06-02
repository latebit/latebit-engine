#pragma once

#include <istream>
using namespace std;

// Reads a stream line by line and returns a string
// accounting for different line endings
auto getLine(istream* file, char delimiter = '\n') -> string;

// Gets next line skipping all comment lines in between
auto getNextNonCommentLine(istream* stream, char commentChar = '#',
                           char delimiter = '\n') -> string;