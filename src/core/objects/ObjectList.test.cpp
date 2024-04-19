#include "core/objects/ObjectList.h"

#include "../../../test/lib/test.h"

using namespace lb;

void fullList() {
  ObjectList subject;
  Object item;

  for (int i = 0; i < MAX_SIZE; i++) {
    Object obj;
    subject.insert(&obj);
  }

  assert("is full", subject.isFull());
  assertEq("has MAX_SIZE elements", subject.getCount(), MAX_SIZE);
  assertFail("errors upon adding", subject.insert(&item));
}

void manyItems() {
  ObjectList subject;
  Object item, item2, item3;
  subject.insert(&item);
  subject.insert(&item2);
  subject.insert(&item3);
  assertEq("has 3 elements", subject.getCount(), 3);
  assert("is not empty", !subject.isEmpty());
  assert("is not full", !subject.isFull());
  subject.remove(&item);
  assertEq("has 2 elements", subject.getCount(), 2);
  subject.clear();
  assertEq("has 0 elements", subject.getCount(), 0);
  assert("is empty", subject.isEmpty());
}

void oneItem() {
  ObjectList subject;
  Object o;

  subject.insert(&o);
  assertEq("has one element", subject.getCount(), 1);
  assert("is not empty", !subject.isEmpty());
  assert("is not full", !subject.isFull());
}

void emptyList() {
  ObjectList subject;
  Object o;

  assertEq("has no elements", subject.getCount(), 0);
  assert("is empty", subject.isEmpty());
  assert("is not full", !subject.isFull());
  assertFail("errors upon removing", subject.remove(&o));
}

void find() {
  ObjectList subject;
  Object item, item2;
  subject.insert(&item);
  assertEq("finds item", subject.find(&item), 0);
  assertEq("does not find item", subject.find(&item2), -1);
}

void concat() {
  ObjectList subject;
  ObjectList other;
  Object item, item2, item3;
  subject.insert(&item);
  other.insert(&item2);
  other.insert(&item3);
  auto result = subject + other;
  assertEq("has 3 elements", result.getCount(), 3);
  assertEq("finds item", result.find(&item), 0);
  assertEq("finds item2", result.find(&item2), 1);
  assertEq("finds item3", result.find(&item3), 2);
}

auto main() -> int {
  test("empty list", emptyList);
  test("one item", oneItem);
  test("many items", manyItems);
  test("full list", fullList);
  test("find", find);
  test("concat (+)", concat);

  return report();
}