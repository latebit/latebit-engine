#include "ObjectList.h"

#include "../lib/test.h"

using namespace df;

void ObjectList_fullList_test() {
  ObjectList subject;
  Object item;

  for (int i = 0; i < MAX_SIZE; i++) {
    subject.insert(new Object);
  }

  assert("is full", subject.isFull());
  assert_int("has MAX_SIZE elements", subject.getCount(), MAX_SIZE);
  assert_fail("errors upon adding", subject.insert(new Object));
}

void ObjectList_manyItems_test() {
  ObjectList subject;
  Object item;
  subject.insert(&item);
  subject.insert(new Object);
  subject.insert(new Object);
  assert_int("has 3 elements", subject.getCount(), 3);
  assert("is not empty", !subject.isEmpty());
  assert("is not full", !subject.isFull());
  subject.remove(&item);
  assert_int("has 2 elements", subject.getCount(), 2);
  subject.clear();
  assert_int("has 0 elements", subject.getCount(), 0);
  assert("is empty", subject.isEmpty());
}

void ObjectList_oneItem_test() {
  ObjectList subject;
  subject.insert(new Object);
  assert_int("has one element", subject.getCount(), 1);
  assert("is not empty", !subject.isEmpty());
  assert("is not full", !subject.isFull());
}

void ObjectList_emptyList_test() {
  ObjectList subject;
  assert_int("has no elements", subject.getCount(), 0);
  assert("is empty", subject.isEmpty());
  assert("is not full", !subject.isFull());

  assert_fail("errors upon removing", subject.remove(new Object));
}

void ObjectList_find_test() {
  ObjectList subject;
  Object item;
  subject.insert(&item);
  assert_int("finds item", subject.find(&item), 0);
  assert_int("does not find item", subject.find(new Object), -1);
}

void ObjectList_test() {
  test("empty list", ObjectList_emptyList_test);
  test("one item", ObjectList_oneItem_test);
  test("many items", ObjectList_manyItems_test);
  test("full list", ObjectList_fullList_test);
  test("find", ObjectList_find_test);
}