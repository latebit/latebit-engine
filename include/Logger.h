#pragma once

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>

#include "Manager.h"
#include "utils.h"

using namespace std;

#define Log lb::Logger::getInstance()
#define Debug(a) lb::Logger::getInstance().debug(a)

namespace lb {

enum LogLevel { ERROR, WARNING, INFO, DEBUG };
enum LogDestination { LOG_FILE, STDOUT };

const string LOGFILE_NAME = "latebits.log";

class Logger {
 public:
  ~Logger();
  static auto getInstance() -> Logger &;
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

  void setLevel(LogLevel level);
  void setDestination(LogDestination destination);

  void error(const string fmt, ...) const;
  void warning(const string fmt, ...) const;
  void info(const string fmt, ...) const;
  void debug(const string fmt, ...) const;

 private:
  Logger();
  // Log level
  LogLevel level = INFO;
  // Log destination
  unique_ptr<std::ostream> output =
    make_unique<ofstream>(LOGFILE_NAME, ios::out);

  // Writes a log line to the chosen destination with a format string
  void logf(LogLevel level, const string fmt, va_list args) const;

  [[nodiscard]] auto getLevelString(LogLevel level) const -> string;
};

}  // namespace lb
