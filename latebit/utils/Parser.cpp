#include "Parser.h"

#include <regex>
#include <sstream>

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

auto getNextNumber(istream* stream, char commentChar) -> int {
  try {
    string line = getNextNonCommentLine(stream, commentChar);
    if (line.empty()) {
      return -1;
    }
    smatch match;
    std::regex numberWithComments("^\\d+\\s*" + string(1, commentChar) +
                                  "*.*$");
    if (std::regex_search(line, match, numberWithComments)) {
      auto result = match.str();
      return stoi(result);
    } else {
      return -1;
    }
  } catch (...) {
    return -1;
  }
}

auto makeRangeValidationMessage(int value, int max, int min) -> string {
  std::ostringstream oss;
  if (max == min) {
    oss << "Expected " << min << ", got " << value;
  } else {
    oss << "Expected a number " << min << "-" << max << ", got " << value;
  }
  return oss.str();
}