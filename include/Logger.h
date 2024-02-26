#pragma once

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

  template <typename... Args>
  void error(Args &&...args) const {
    log(ERROR, args...);
  }

  template <typename... Args>
  void warning(Args &&...args) const {
    log(WARNING, args...);
  }

  template <typename... Args>
  void info(Args &&...args) const {
    log(INFO, args...);
  }

  template <typename... Args>
  void debug(Args &&...args) const {
    log(DEBUG, args...);
  }

 private:
  Logger();
  // Log level
  LogLevel level = INFO;
  // Log destination
  unique_ptr<std::ostream> output =
    make_unique<ofstream>(LOGFILE_NAME, ios::out);

  // Writes a log line to the chosen destination
  template <typename... Args>
  void log(LogLevel level, Args &&...args) const {
    if (level <= this->level) {
      string time = getTimeString();

      string logLevel = "UNKNOWN";
      switch (level) {
        case LogLevel::DEBUG:
          logLevel = "DEBUG";
          break;
        case LogLevel::INFO:
          logLevel = "INFO";
          break;
        case LogLevel::WARNING:
          logLevel = "WARNING";
          break;
        case LogLevel::ERROR:
          logLevel = "ERROR";
          break;
      }

      (*output) << time << " [" << logLevel << "] ";
      (((*output) << args << ' '), ...) << endl;
    }
  }
};

}  // namespace lb
