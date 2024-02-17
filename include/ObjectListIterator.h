#pragma once

#include "Object.h"
#include "ObjectList.h"

namespace lb {

class ObjectList;

class ObjectListIterator {
 private:
  // Index of the current object in the list
  int currentIndex;
  // List to iterate through
  const ObjectList *list;

 public:
  // Creates an iterator for the given list
  ObjectListIterator(const ObjectList *l);
  ObjectListIterator() = delete;
  // Resets the current index to 0
  void first();
  // Increments the current index by 1
  void next();
  // Returns true if the current index is greater than or equal than the list
  // length
  [[nodiscard]] auto isDone() const -> bool;
  // Returns the object at the current index in the list
  [[nodiscard]] auto currentObject() const -> Object *;
};
}  // namespace lb
