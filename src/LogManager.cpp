#include "LogManager.h"

#include <unistd.h>

#include <cstdarg>

#include "utils.h"

namespace lb {

const char* LOGFILE_NAME = "dragonfly.log";

LogManager::LogManager() {
  setType("LogManager");
  this->file = nullptr;
  this->flush = false;
  startUp();
  writeLog("LogManager::LogManager(): Created LogManager");
}

LogManager::~LogManager() = default;

auto LogManager::startUp() -> int {
  this->file = std::fopen(LOGFILE_NAME, "w");

  if (this->file == nullptr) {
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
  if (this->file != nullptr) std::fclose(this->file);
  this->file = nullptr;
  Manager::shutDown();
}

void LogManager::setFlush(bool flush) { this->flush = flush; }

auto LogManager::writeLog(const char* fmt, ...) const -> int {
  if (this->file == nullptr) {
    stdoutLog("LogManager::writeLog(): Warning! Log file not open");
    return -1;
  }

  fprintf(this->file, "[%s] ", getTimeString().c_str());
  va_list args;
  va_start(args, fmt);
  vfprintf(this->file, fmt, args);
  va_end(args);
  fprintf(this->file, "\n");

  if (this->flush) {
    fflush(this->file);
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
}  // namespace lb
