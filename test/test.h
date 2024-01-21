#pragma once

#include <string>

#include "../include/Box.h"
#include "../include/Clock.h"
#include "../include/Vector.h"

using namespace df;
using namespace std;

// Number of assertions in this run
auto getAssertions() -> string;
auto getClock() -> Clock;

auto assert(const string name, bool assertion,
            const string message = "wanted true got false") -> int;
auto assert_string(string name, string want, string got) -> int;
auto assert_regex(string name, string want, string pattern) -> int;
auto assert_float(string name, float got, float want) -> int;
auto assert_int(string name, int got, int want) -> int;
auto assert_ok(string name, int got) -> int;
auto assert_fail(string name, int got) -> int;

// Project-specific assertions
auto assert_vector(string name, Vector got, Vector want) -> int;
auto assert_box(string name, Box got, Box want) -> int;

auto suite(string name, int (*test)()) -> int;
auto test(string name, int (*test)()) -> int;