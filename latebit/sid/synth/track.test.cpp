#include "track.h"

#include "latebit/sid/synth/oscillator.h"
#include "test/lib/test.h"

using namespace sid;

void notes() {
  Note n = Note(48, 8, TRIANGLE, NONE);
  Note m = Note(48, 8, TRIANGLE, NONE);
  Note r = Note::rest();
  Note s = Note::rest();
  Note i = Note::invalid();
  Note j = Note::invalid();

  assert("compares same note", n.isSame(n));
  assert("compares different notes", !n.isSame(m));
  assert("deep compares different notes", n.isEqual(m));

  assert("compares same rest", r.isSame(r));
  assert("compares different rest", r.isSame(s) && r.isEqual(r));
  assert("detects rest", r.isRest());
  assert("does not detect rest (invalid)", !i.isRest());
  assert("does not detect rest (note)", !n.isRest());

  assert("compares same invalid", i.isSame(i));
  assert("compares different invalid", i.isSame(j) && i.isEqual(j));
  assert("detects invalid", i.isInvalid());
  assert("does not detect invalid (invalid)", !r.isInvalid());
  assert("does not detect invalid (note)", !n.isInvalid());
}

auto main() -> int {
  test("notes", notes);
  return report();
}
