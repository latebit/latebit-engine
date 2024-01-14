#include "ObjectListIterator.h"

namespace df {
ObjectListIterator::ObjectListIterator(const ObjectList* l) {
  this->currentIndex = 0;
  this->list = l;
}

void ObjectListIterator::first() { this->currentIndex = 0; }

void ObjectListIterator::next() {
  this->currentIndex = std::min(this->currentIndex + 1, this->list->count);
}

bool ObjectListIterator::isDone() const {
  return this->currentIndex >= this->list->count;
}

Object* ObjectListIterator::currentObject() const {
  if (this->currentIndex < this->list->count) {
    return this->list->obj[this->currentIndex];
  }
  return nullptr;
}

}  // namespace df
