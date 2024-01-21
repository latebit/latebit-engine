#include "ObjectListIterator.h"

#include "../lib/test.h"
#include "Object.h"

void ObjectListIterator_iteration_test() {
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
    assert_int("gets object", subject.currentObject()->getId(), id);
    id++;
  }
  assert("is done", subject.isDone());
  assert("returns null pointer", subject.currentObject() == nullptr);

  list.insert(&obj1);
  assert_int("retrieves object after starting iteration",
             subject.currentObject()->getId(), obj1.getId());
  list.remove(&obj2);
  assert("returns null pointer", subject.currentObject() == nullptr);

  subject.first();
  assert_int("returns to first item", subject.currentObject()->getId(),
             obj0.getId());
}

void ObjectListIterator_emptyList_test() {
  ObjectList list;
  ObjectListIterator subject(&list);

  for (subject.first(); !subject.isDone(); subject.next()) {
    assert("does not go here", false);
  }
  assert("returns null pointer", subject.currentObject() == nullptr);
}

void ObjectListIterator_test() {
  test("iteration", ObjectListIterator_iteration_test);
  test("empty list", ObjectListIterator_emptyList_test);
}