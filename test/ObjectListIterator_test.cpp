#include "../include/ObjectListIterator.h"

#include <iostream>

#include "../include/Object.h"
#include "ObjectListIterator_test.h"
#include "ObjectList_test.h"
#include "test.h"

int ObjectListIterator_iteration_test() {
  int result = 0;
  std::cout << "ObjectListIterator_iteration_test\n";

  df::ObjectListIterator *subject;
  auto list = new df::ObjectList;
  auto obj0 = new df::Object;
  auto obj1 = new df::Object;
  auto obj2 = new df::Object;
  list->insert(obj0);
  list->insert(obj1);
  list->insert(obj2);
  subject = new df::ObjectListIterator(list);

  int id = obj0->getId();
  for (subject->first(); !subject->isDone(); subject->next()) {
    result += assert_int("gets object", subject->currentObject()->getId(), id);
    id++;
  }
  result += assert("is done", subject->isDone());
  result += assert("returns null pointer", subject->currentObject() == nullptr);

  list->insert(obj1);
  result += assert_int("retrieves object after starting iteration",
                       subject->currentObject()->getId(), obj1->getId());
  list->remove(obj2);
  result += assert("returns null pointer", subject->currentObject() == nullptr);

  subject->first();
  result += assert_int("returns to first item",
                       subject->currentObject()->getId(), obj0->getId());

  return result;
}

int ObjectListIterator_emptyList_test() {
  int result = 0;
  std::cout << "ObjectListIterator_emptyList_test\n";
  auto list = new df::ObjectList;
  auto subject = new df::ObjectListIterator(list);

  for (subject->first(); !subject->isDone(); subject->next()) {
    result += assert("does not go here", false);
  }
  result += assert("returns null pointer", subject->currentObject() == nullptr);

  return result;
}

int ObjectListIterator_test() {
  int result = 0;

  result += ObjectListIterator_iteration_test();
  result += ObjectListIterator_emptyList_test();

  return result;
}