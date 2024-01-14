#include "../include/ObjectList.h"

#include <iostream>

#include "ObjectList_test.h"
#include "test.h"

auto ObjectList_fullList_test() -> int {
  std::cout << "ObjectList_fullList_test\n";
  int result = 0;
  auto subject = new df::ObjectList;
  auto item = new df::Object;
  for (int i = 0; i < df::MAX_SIZE; i++) {
    subject->insert(new df::Object);
  }

  result += assert("is full", subject->isFull());
  result +=
    assert_int("has MAX_SIZE elements", subject->getCount(), df::MAX_SIZE);

  result +=
    assert_int("errors upon adding", subject->insert(new df::Object), -1);

  return result;
}

auto ObjectList_manyItems_test() -> int {
  std::cout << "ObjectList_manyItems_test\n";
  int result = 0;
  auto subject = new df::ObjectList;
  auto item = new df::Object;
  subject->insert(item);
  subject->insert(new df::Object);
  subject->insert(new df::Object);
  result += assert_int("has 3 elements", subject->getCount(), 3);
  result += assert("is not empty", !subject->isEmpty());
  result += assert("is not full", !subject->isFull());
  subject->remove(item);
  result += assert_int("has 2 elements", subject->getCount(), 2);
  subject->clear();
  result += assert_int("has 0 elements", subject->getCount(), 0);
  result += assert("is empty", subject->isEmpty());

  return result;
}

auto ObjectList_oneItem_test() -> int {
  std::cout << "ObjectList_oneItem_test\n";
  int result = 0;
  auto subject = new df::ObjectList;
  subject->insert(new df::Object);
  result += assert_int("has one element", subject->getCount(), 1);
  result += assert("is not empty", !subject->isEmpty());
  result += assert("is not full", !subject->isFull());
  return result;
}

auto ObjectList_emptyList_test() -> int {
  std::cout << "ObjectList_emptyList_test\n";
  int result = 0;
  auto subject = new df::ObjectList;
  result += assert_int("has no elements", subject->getCount(), 0);
  result += assert("is empty", subject->isEmpty());
  result += assert("is not full", !subject->isFull());
  result +=
    assert_int("errors upon removing", subject->remove(new df::Object), -1);
  return result;
}

auto ObjectList_find_test() -> int {
  std::cout << "ObjectList_find_test\n";
  int result = 0;
  auto subject = new df::ObjectList;
  auto item = new df::Object;
  subject->insert(item);
  result += assert_int("finds item", subject->find(item), 0);
  result += assert_int("does not find item", subject->find(new df::Object), -1);
  return result;
}

auto ObjectList_test() -> int {
  int result = 0;

  result += ObjectList_emptyList_test();
  result += ObjectList_oneItem_test();
  result += ObjectList_manyItems_test();
  result += ObjectList_fullList_test();
  result += ObjectList_find_test();

  return result;
}