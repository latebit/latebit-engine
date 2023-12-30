#ifndef __TEST_H__
#define __TEST_H__

#include <string>

int assert(const std::string name, bool assertion,
           const std::string message = "wanted true got false");
int assert_string(std::string name, std::string want, std::string got);
int assert_regex(std::string name, std::string want, std::string pattern);
int assert_float(std::string name, float got, float want);

#endif