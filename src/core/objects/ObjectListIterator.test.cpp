#include "core/objects/ObjectListIterator.h"

#include "../../../test/lib/test.h"
#include "core/objects/Object.h"

void iteration() {
  ObjectList list;
  Object obj0;
  Object obj1;
  Object obj2;

  list.insert(&obj0);
  list.insert(&obj1);
  list.insert(&obj2);

  ObjectListIterator subject(&list);

  int id = obj0.getId();
  for (subject.first(); !subject.isDone(); subject.next()) {
    assertEq("gets object", subject.currentObject()->getId(), id);
    id++;
  }
  assert("is done", subject.isDone());
  assert("returns null pointer", subject.currentObject() == nullptr);

  list.insert(&obj1);
  assertEq("retrieves object after starting iteration",
           subject.currentObject()->getId(), obj1.getId());
  list.remove(&obj2);
  assert("returns null pointer", subject.currentObject() == nullptr);

  subject.first();
  assertEq("returns to first item", subject.currentObject()->getId(),
           obj0.getId());
}

void emptyList() {
  ObjectList list;
  ObjectListIterator subject(&list);

  for (subject.first(); !subject.isDone(); subject.next()) {
    assert("does not go here", false);
  }
  assert("returns null pointer", subject.currentObject() == nullptr);
}

auto main() -> int {
  test("iteration", iteration);
  test("empty list", emptyList);
  return report();
}