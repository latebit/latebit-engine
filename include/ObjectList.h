#pragma once

#include "Object.h"

namespace df {

const int MAX_SIZE = 2000;

class ObjectListIterator;

class ObjectList {
 private:
  int count;
  Object *p_obj[MAX_SIZE];

 public:
  friend class ObjectListIterator;

  ObjectList();

  int insert(Object *p_o);

  int remove(Object *p_o);

  void clear();

  int getCount() const;
  bool isEmpty() const;
  bool isFull() const;
  int find(Object *p_o) const;
};

}  // namespace df