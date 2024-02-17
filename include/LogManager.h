#pragma once

#include "Manager.h"

#define LM lb::LogManager::getInstance()

namespace lb {

extern const char *LOGFILE_NAME;

class LogManager : public Manager {
 private:
  LogManager();
  // If true, the logs will be flushed after each write
  bool flush;
  // File pointer to the logfile
  FILE *file;

 public:
  ~LogManager() override;
  static auto getInstance() -> LogManager &;
  LogManager(LogManager const &) = delete;
  void operator=(LogManager const &) = delete;

  auto startUp() -> int override;
  void shutDown() override;

  // Set flush of logfile after each write
  void setFlush(bool flush = true);

  // Write to logfile
  auto writeLog(const char *fmt, ...) const -> int;

  // Fallback log to be used when the logger is not yet initialized
  void stdoutLog(const char *fmt, ...) const;
};

}  // namespace lb
