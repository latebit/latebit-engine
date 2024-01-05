#ifndef __TEST_H__
#define __TEST_H__

#include <string>

#include "../src/Vector.h"

// Number of assertions in this run
std::string getAssertions();

int assert(const std::string name, bool assertion,
           const std::string message = "wanted true got false");
int assert_string(std::string name, std::string want, std::string got);
int assert_regex(std::string name, std::string want, std::string pattern);
int assert_float(std::string name, float got, float want);
int assert_int(std::string name, int got, int want);
int assert_vector(std::string name, df::Vector p_got, df::Vector p_want);
int assert_ok(std::string name, int got);
int assert_fail(std::string name, int got);

#endif