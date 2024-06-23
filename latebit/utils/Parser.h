#pragma once

#include <istream>
using namespace std;

// Reads a stream line by line and returns a string
// accounting for different line endings
auto getLine(istream* file, char delimiter = '\n') -> string;

// Gets next line skipping all comment lines in between
auto getNextNonCommentLine(istream* stream, char commentChar = '#',
                           char delimiter = '\n') -> string;

// Gets the first number in the stream skipping all comment lines
// and catering for lines with comments at the end
auto getNextNumber(istream* stream, char commentChar = '#') -> int;

// Returns a message indicating the value is out of range
// Generally used after getNextNumber() to validate the value
auto makeRangeValidationMessage(int value, int max, int min = 1) -> string;