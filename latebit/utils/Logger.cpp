#include "Logger.h"

#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace lb {
using std::make_unique;

// Returns current time as a string
auto getTimeString() -> string {
  time_t now = time(nullptr);
  tm* lt = localtime(&now);

  ostringstream stream;
  stream << setfill('0') << setw(2) << lt->tm_hour << ":" << setfill('0')
         << setw(2) << lt->tm_min << ":" << setfill('0') << setw(2)
         << lt->tm_sec;

  return stream.str();
}

Logger::Logger() = default;
Logger::~Logger() = default;

auto Logger::getInstance() -> Logger& {
  static Logger instance;
  return instance;
}

void Logger::setDestination(LogDestination::LogDestination d) {
  if (d == LogDestination::STDOUT) {
    this->output = make_unique<ostream>(cout.rdbuf());
  } else {
    this->output = make_unique<ofstream>(LOGFILE_NAME, ios::out);
  }
}

void Logger::setLevel(LogLevel::LogLevel l) { this->level = l; }

auto Logger::getLevelString(LogLevel::LogLevel l) const -> string {
  switch (l) {
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::DEBUG:
      return "DEBUG";
  }
  return "UNKNOWN";
}

void Logger::logf(LogLevel::LogLevel level, const string fmt,
                  va_list args) const {
  if (level <= this->level) {
    string time = getTimeString();
    string logLevel = getLevelString(level);

    char buffer[1024];  // NOLINT
    snprintf(buffer, 1024, "%s ", getTimeString().c_str());
    vsnprintf(buffer, 1024, fmt.c_str(), args);

    (*output) << time << " [" << logLevel << "] " << buffer << endl;
  }
}

void Logger::error(const string message, ...) const {
  va_list args;
  va_start(args, message);
  logf(LogLevel::ERROR, message, args);
  va_end(args);
}

void Logger::warning(const string message, ...) const {
  va_list args;
  va_start(args, message);
  logf(LogLevel::WARNING, message, args);
  va_end(args);
}

void Logger::info(const string message, ...) const {
  va_list args;
  va_start(args, message);
  logf(LogLevel::INFO, message, args);
  va_end(args);
}

void Logger::debug(const string message, ...) const {
  va_list args;
  va_start(args, message);
  logf(LogLevel::DEBUG, message, args);
  va_end(args);
}

}  // namespace lb