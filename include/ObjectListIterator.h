#pragma once

#include "Object.h"
#include "ObjectList.h"

namespace df {

class ObjectList;

class ObjectListIterator {
 private:
  ObjectListIterator();
  int currentIndex;
  const ObjectList *list;

 public:
  ObjectListIterator(const ObjectList *l);

  void first();
  void next();
  bool isDone() const;

  Object *currentObject() const;
};
}  // namespace df
