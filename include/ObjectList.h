#pragma once

#include "Object.h"

using namespace std;

namespace df {

const int MAX_SIZE = 2000;

class ObjectListIterator;

class ObjectList {
 private:
  int count = 0;
  array<Object *, MAX_SIZE> obj;

 public:
  friend class ObjectListIterator;

  ObjectList();
  auto operator+(ObjectList const &other) const -> ObjectList;

  auto insert(Object *o) -> int;

  auto remove(Object *o) -> int;

  void clear();

  [[nodiscard]] auto getCount() const -> int;
  [[nodiscard]] auto isEmpty() const -> bool;
  [[nodiscard]] auto isFull() const -> bool;
  auto find(Object *o) const -> int;
};

}  // namespace df