#include "test.h"

#include <iostream>
#include <regex>
#include <sstream>

#include "test.h"

int assert(const std::string name, bool assertion, const std::string message) {
  if (!assertion) {
    std::cout << "FAIL: " << name << std::endl;
    std::cout << "      " << message << std::endl;
    return 1;
  }

  std::cout << " OK : " << name << std::endl;
  return 0;
}

int assert_string(std::string name, std::string got, std::string want) {
  const bool assertion = got.compare(want) == 0;

  std::ostringstream message;
  message << "wanted \'" << want << "\' got \'" << got << "\'";
  std::string result_message = message.str();

  return assert(name, assertion, result_message);
}

int assert_regex(std::string name, std::string want, std::string pattern) {
  std::regex expression(pattern);
  const bool assertion = std::regex_match(want, expression);

  std::ostringstream message;
  message << "string \'" << want << "\' doesn't match pattern \'" << pattern
          << "\'";
  std::string result_message = message.str();

  return assert(name, assertion, result_message);
}
