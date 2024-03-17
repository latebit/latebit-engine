#include "Configuration.h"

#include <algorithm>
#include <fstream>
#include <string>

#include "Logger.h"
#include "utils.h"

// Trims whitespace from both ends
auto trim(const string &str) -> string {
  auto start = find_if_not(str.begin(), str.end(), ::isspace);
  auto end = find_if_not(str.rbegin(), str.rend(), ::isspace).base();
  return (start < end) ? string(start, end) : string();
}

// Takes a string value and attempts a conversion to int. Uses a default value
// on failure.
auto intOrDefault(string value, int def) -> int {
  if (value.empty()) return def;

  try {
    auto result = stoi(value);
    if (result < 0) return def;
    return result;
  } catch (const invalid_argument &e) {
    return def;
  }
}

// Takes a string value and returns it or a default value if it's empty.
auto stringOrDefault(string value, string def) -> string {
  if (value.empty()) return def;
  return value;
}

template <unsigned long T>
auto join(array<string, T> array) -> string {
  string result;
  for (auto &s : array) result += s + ", ";
  return result.substr(0, result.size() - 2);
}

int Configuration::frameRate = DEFAULT_FRAME_RATE;
string Configuration::title = DEFAULT_TITLE;

auto Configuration::getMaxFrameRate() -> int { return frameRate; }

auto Configuration::getInitialWindowTitle() -> string { return title; }

auto Configuration::fromFile(string filename) -> int {
  ifstream file(filename);

  if (!file.is_open()) {
    Log.error("Configuration::fromFile(): Could not open file %s",
              filename.c_str());
    return -1;
  }

  string line;
  while (!file.eof()) {
    line = getLine(&file);

    // Ignore empty lines and comments
    if (line.empty() || line.starts_with('#')) continue;

    string key = trim(line.substr(0, line.find('=')));
    string value = trim(line.substr(line.find('=') + 1));

    if (value.empty()) continue;

    try {
      if (key == CONFIG_KEYS[0]) {
        frameRate = intOrDefault(value, frameRate);
      } else if (key == CONFIG_KEYS[1]) {
        title = stringOrDefault(value, title);
      } else {
        Log.warning(
          "Configuration::fromFile(): Unknown key %s. Expected one of %s.",
          key.c_str(), join(CONFIG_KEYS).c_str());
      }
    } catch (const invalid_argument &e) {
      Log.warning("Configuration::fromFile(): Invalid value for key %s: %s",
                  key.c_str(), value.c_str());
    }
  }

  file.close();
  return 0;
}

auto Configuration::reset() -> int {
  frameRate = DEFAULT_FRAME_RATE;
  title = DEFAULT_TITLE;
  return 0;
}