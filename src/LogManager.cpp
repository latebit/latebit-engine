#include "LogManager.h"

#include <unistd.h>

#include "utils.h"

namespace df {

const char* LOGFILE_NAME = "dragonfly.log";

LogManager::LogManager() {
  setType("LogManager");
  m_p_f = nullptr;
  m_do_flush = false;
  startUp();
  stdoutLog("LogManager::LogManager(): Created LogManager");
}

void LogManager::stdoutLog(const char* fmt, ...) const {
  printf("[%s] ", getTimeString());
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

LogManager::~LogManager() {
  if (isStarted()) {
    shutDown();
  }
}

int LogManager::startUp() {
  m_p_f = std::fopen(LOGFILE_NAME, "w");

  if (m_p_f == nullptr) {
    return -1;
  }

  writeLog("LogManager::startUp(): Started successfully");
  return Manager::startUp();
}

LogManager& LogManager::getInstance() {
  static LogManager instance;
  return instance;
}

void LogManager::shutDown() {
  LM.writeLog("LogManager::shutDown(): Shutting down");
  if (m_p_f != nullptr) std::fclose(m_p_f);
  m_p_f = nullptr;
  stdoutLog("LogManager::shutDown(): Shut down successfully");
  Manager::shutDown();
}

void LogManager::setFlush(bool do_flush) { m_do_flush = do_flush; }

int LogManager::writeLog(const char* fmt, ...) const {
  if (m_p_f == nullptr) {
    stdoutLog("LogManager::writeLog(): Warning! Log file not open");
    return -1;
  }

  fprintf(m_p_f, "[%s] ", getTimeString());
  va_list args;
  va_start(args, fmt);
  vfprintf(m_p_f, fmt, args);
  va_end(args);
  fprintf(m_p_f, "\n");

  if (m_do_flush) {
    fflush(m_p_f);
  }

  return 0;
}
}  // namespace df
