#include "Parser.h"

auto getLine(istream* file) -> string {
  string line;

  getline(*file, line);

  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }

  return line;
}

auto getNonCommentedLine(istream* stream, char commentChar) -> string {
  string line = getLine(stream);

  while (!line.empty()) {
    if (line[0] != commentChar) {
      return line;
    }
    line = getLine(stream);
  }

  return "";
}