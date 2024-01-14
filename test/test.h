#pragma once

#include <string>

#include "../include/Box.h"
#include "../include/Vector.h"

using namespace df;
using namespace std;

// Number of assertions in this run
string getAssertions();

int assert(const string name, bool assertion,
           const string message = "wanted true got false");
int assert_string(string name, string want, string got);
int assert_regex(string name, string want, string pattern);
int assert_float(string name, float got, float want);
int assert_int(string name, int got, int want);
int assert_ok(string name, int got);
int assert_fail(string name, int got);

// Project-specific assertions
int assert_vector(string name, Vector got, Vector want);
int assert_box(string name, Box got, Box want);