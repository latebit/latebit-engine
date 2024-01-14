#pragma once

#include "Object.h"

namespace df {

const int MAX_SIZE = 2000;

class ObjectListIterator;

class ObjectList {
 private:
  int count;
  Object *obj[MAX_SIZE];

 public:
  friend class ObjectListIterator;

  ObjectList();

  auto insert(Object *o) -> int;

  auto remove(Object *o) -> int;

  void clear();

  auto getCount() const -> int;
  auto isEmpty() const -> bool;
  auto isFull() const -> bool;
  auto find(Object *o) const -> int;
};

}  // namespace df