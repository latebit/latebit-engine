#pragma once

#include "Object.h"

using namespace std;

namespace lb {

const int MAX_SIZE = 2000;

class ObjectListIterator;

class ObjectList {
 private:
  // Amount of objects in the list
  int count = 0;
  // Array of objects
  array<Object *, MAX_SIZE> obj = {};

 public:
  friend class ObjectListIterator;

  ObjectList();
  // Concat two lists
  auto operator+(ObjectList const &other) const -> ObjectList;
  // Insert object pointer at the end of the list
  auto insert(Object *o) -> int;
  // Removes object from the list
  auto remove(Object *o) -> int;
  // Clear list (setting count to 0)
  void clear();
  // Returns number of objects in list
  [[nodiscard]] auto getCount() const -> int;
  // Returns true if list is empty
  [[nodiscard]] auto isEmpty() const -> bool;
  // Returns true if list is full
  [[nodiscard]] auto isFull() const -> bool;
  // Returns index of the object in the list, -1 if not found
  auto find(Object *o) const -> int;
};

}  // namespace lb