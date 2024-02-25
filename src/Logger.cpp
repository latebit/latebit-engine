#include "Logger.h"

#include <unistd.h>

#include <iostream>

using namespace std;

namespace lb {
using std::make_unique;

Logger::Logger() = default;
Logger::~Logger() = default;

auto Logger::getInstance() -> Logger& {
  static Logger instance;
  return instance;
}

void Logger::setDestination(LogDestination d) {
  if (d == STDOUT) {
    this->output = make_unique<ostream>(cout.rdbuf());
  } else {
    this->output = make_unique<ofstream>(LOGFILE_NAME, ios::out);
  }
}

void Logger::setLevel(LogLevel l) { this->level = l; }

}  // namespace lb