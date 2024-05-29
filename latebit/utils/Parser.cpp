#include "Parser.h"

auto getLine(istream* file, char delimiter) -> string {
  string line;

  getline(*file, line, delimiter);

  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }

  return line;
}

auto getNextNonCommentLine(istream* stream, char commentChar,
                           char delimiter) -> string {
  string line = getLine(stream, delimiter);

  while (!line.empty()) {
    if (line[0] != commentChar) {
      return line;
    }
    line = getLine(stream);
  }

  return "";
}