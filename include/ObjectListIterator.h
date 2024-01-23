#pragma once

#include "Object.h"
#include "ObjectList.h"

namespace df {

class ObjectList;

class ObjectListIterator {
 private:
  ObjectListIterator() = delete;
  int currentIndex;
  const ObjectList *list;

 public:
  ObjectListIterator(const ObjectList *l);

  void first();
  void next();
  [[nodiscard]] auto isDone() const -> bool;

  [[nodiscard]] auto currentObject() const -> Object *;
};
}  // namespace df
