#pragma once

#include <string>

#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/utils/Clock.h"

using namespace lb;
using namespace std;

const string FIXTURES_FOLDER = "../../../test/fixtures";

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

// Report test stats at the end of a run
// It returns the number of failed tests and can therefore be used as status
// code at the end of an execution to, for example, fail CI
auto report() -> int;