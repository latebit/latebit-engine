#pragma once

#include <string>

#include "core/geometry/Box.h"
#include "core/geometry/Vector.h"
#include "core/utils/Clock.h"

using namespace lb;
using namespace std;

auto getAssertions() -> string;
auto getFailedAssertions() -> int;
auto getClock() -> Clock;

auto assert(const string name, bool assertion,
            const string message = "wanted true got false") -> int;
auto assert_string(string name, string want, string got) -> int;
auto assert_regex(string name, string want, string pattern) -> int;
auto assert_float(string name, float got, float want) -> int;
auto assert_lt(string name, float got, float want) -> int;
auto assert_int(string name, int got, int want) -> int;
auto assert_ok(string name, int got) -> int;
auto assert_fail(string name, int got) -> int;

// Project-specific assertions
auto assert_vector(string name, Vector got, Vector want) -> int;
auto assert_box(string name, Box got, Box want) -> int;

auto suite(string name, int (*test)()) -> int;
auto suite(string name, void (*test)()) -> void;

auto test(string name, int (*test)()) -> int;
auto test(string name, void (*test)()) -> void;