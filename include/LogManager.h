#pragma once

#include "Manager.h"

#define LM df::LogManager::getInstance()

namespace df {

extern const char *LOGFILE_NAME;

class LogManager : public Manager {
 private:
  LogManager();
  LogManager(LogManager const &);
  void operator=(LogManager const &);
  bool do_flush;
  FILE *f;

 public:
  ~LogManager() override;
  static auto getInstance() -> LogManager &;
  auto startUp() -> int override;
  void shutDown() override;
  void setFlush(bool do_flush = true);
  auto writeLog(const char *fmt, ...) const -> int;
  // To be used when the logger is not yet initialized
  void stdoutLog(const char *fmt, ...) const;
};

}  // namespace df
