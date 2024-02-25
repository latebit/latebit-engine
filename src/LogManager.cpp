#include "LogManager.h"

#include <unistd.h>

#include <iostream>

using namespace std;

namespace lb {
using std::make_unique;

const char* LOGFILE_NAME = "latebits.log";

LogManager::LogManager() = default;
LogManager::~LogManager() = default;

auto LogManager::getInstance() -> LogManager& {
  static LogManager instance;
  return instance;
}

void LogManager::setDestination(LogDestination d) {
  if (d == STDOUT) {
    this->output = make_unique<ostream>(cout.rdbuf());
  } else {
    this->output = make_unique<ofstream>(LOGFILE_NAME, ios::out);
  }
}

void LogManager::setLevel(LogLevel l) { this->level = l; }

}  // namespace lb