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

  int insert(Object *o);

  int remove(Object *o);

  void clear();

  int getCount() const;
  bool isEmpty() const;
  bool isFull() const;
  int find(Object *o) const;
};

}  // namespace df