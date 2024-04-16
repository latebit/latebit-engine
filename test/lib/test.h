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

auto assertEq(string name, string want, string got) -> int;
auto assertEq(string name, float got, float want) -> int;
auto assertEq(string name, int got, int want) -> int;
auto assertEq(string name, Vector got, Vector want) -> int;
auto assertEq(string name, Box got, Box want) -> int;

auto assertLessThan(string name, float got, float want) -> int;
auto assertMatch(string name, string want, string pattern) -> int;
auto assertOk(string name, int got) -> int;
auto assertFail(string name, int got) -> int;

auto suite(string name, void (*test)()) -> void;

auto test(string name, void (*test)()) -> void;