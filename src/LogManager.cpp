#include "LogManager.h"

#include <unistd.h>

#include "utils.h"

namespace df {

const char* LOGFILE_NAME = "dragonfly.log";

LogManager::LogManager() {
  setType("LogManager");
  startUp();
}

LogManager::~LogManager() { shutDown(); }

int LogManager::startUp() {
  m_p_f = std::fopen(LOGFILE_NAME, "w");

  if (m_p_f == nullptr) {
    return -1;
  }

  return 0;
}

LogManager& LogManager::getInstance() {
  static LogManager instance;
  return instance;
}

void LogManager::shutDown() {
  if (m_p_f != nullptr) std::fclose(m_p_f);
}

void LogManager::setFlush(bool do_flush) { m_do_flush = do_flush; }

int LogManager::writeLog(const char* fmt, ...) const {
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
