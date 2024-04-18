#include "Parser.h"

auto getLine(istream* file) -> string {
  string line;

  getline(*file, line);

  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }

  return line;
}