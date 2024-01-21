#include "LogManager.h"

#include <unistd.h>

#include <cstdarg>

#include "utils.h"

namespace df {

const char* LOGFILE_NAME = "dragonfly.log";

LogManager::LogManager() {
  setType("LogManager");
  this->f = nullptr;
  this->do_flush = false;
  startUp();
  writeLog("LogManager::LogManager(): Created LogManager");
}

LogManager::~LogManager() = default;

auto LogManager::startUp() -> int {
  this->f = std::fopen(LOGFILE_NAME, "w");

  if (this->f == nullptr) {
    return -1;
  }

  writeLog("LogManager::startUp(): Started successfully");
  return Manager::startUp();
}

auto LogManager::getInstance() -> LogManager& {
  static LogManager instance;
  return instance;
}

void LogManager::shutDown() {
  LM.writeLog("LogManager::shutDown(): Shutting down");
  if (this->f != nullptr) std::fclose(this->f);
  this->f = nullptr;
  Manager::shutDown();
}

void LogManager::setFlush(bool do_flush) { this->do_flush = do_flush; }

auto LogManager::writeLog(const char* fmt, ...) const -> int {
  if (this->f == nullptr) {
    stdoutLog("LogManager::writeLog(): Warning! Log file not open");
    return -1;
  }

  fprintf(this->f, "[%s] ", getTimeString().c_str());
  va_list args;
  va_start(args, fmt);
  vfprintf(this->f, fmt, args);
  va_end(args);
  fprintf(this->f, "\n");

  if (this->do_flush) {
    fflush(this->f);
  }

  return 0;
}

void LogManager::stdoutLog(const char* fmt, ...) const {
  printf("[%s] ", getTimeString().c_str());
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}
}  // namespace df
