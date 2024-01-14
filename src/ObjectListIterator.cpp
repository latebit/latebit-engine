#include "ObjectListIterator.h"

namespace df {
ObjectListIterator::ObjectListIterator(const ObjectList* p_l) {
  this->currentIndex = 0;
  this->p_list = p_l;
}

void ObjectListIterator::first() { this->currentIndex = 0; }

void ObjectListIterator::next() {
  this->currentIndex = std::min(this->currentIndex + 1, this->p_list->count);
}

bool ObjectListIterator::isDone() const {
  return this->currentIndex >= this->p_list->count;
}

Object* ObjectListIterator::currentObject() const {
  if (this->currentIndex < this->p_list->count) {
    return this->p_list->p_obj[this->currentIndex];
  }
  return nullptr;
}

}  // namespace df
