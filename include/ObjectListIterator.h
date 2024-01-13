#pragma once

#include "Object.h"
#include "ObjectList.h"

namespace df {

class ObjectList;

class ObjectListIterator {
 private:
  ObjectListIterator();
  int m_current_index;
  const ObjectList *m_p_list;

 public:
  ObjectListIterator(const ObjectList *p_l);

  void first();
  void next();
  bool isDone() const;

  Object *currentObject() const;
};
}  // namespace df
