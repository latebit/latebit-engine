#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>

using namespace std;

#define Log lb::Logger::getInstance()
#define Debug(a) lb::Logger::getInstance().debug(a)

namespace lb {
namespace LogLevel {
enum LogLevel { ERROR, WARNING, INFO, DEBUG };
}
namespace LogDestination {
enum LogDestination { LOG_FILE, STDOUT };
}

const string LOGFILE_NAME = "latebit.log";

class Logger {
 public:
  ~Logger();
  static auto getInstance() -> Logger &;
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

  void setLevel(LogLevel::LogLevel level);
  void setDestination(LogDestination::LogDestination destination);

  // Log line as an error. Always printed
  void error(const string fmt, ...) const;
  // Log line as a warning. Printed if level is WARNING or higher
  void warning(const string fmt, ...) const;
  // Log line as an info. Printed if level is INFO or higher
  void info(const string fmt, ...) const;
  // Log line as a debug. Printed if level is DEBUG or higher
  void debug(const string fmt, ...) const;

 private:
  Logger();
  // Log level
  LogLevel::LogLevel level = LogLevel::INFO;
  // Log destination
  unique_ptr<std::ostream> output =
    make_unique<ofstream>(LOGFILE_NAME, ios::out);

  // Writes a log line to the chosen destination with a format string
  void logf(LogLevel::LogLevel level, const string fmt, va_list args) const;

  // Returns a string version of the current level
  [[nodiscard]] auto getLevelString(LogLevel::LogLevel level) const -> string;
};

}  // namespace lb
