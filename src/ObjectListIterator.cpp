#include "ObjectListIterator.h"

#include <algorithm>

#include "ObjectList.h"

using namespace std;

namespace df {
ObjectListIterator::ObjectListIterator(const ObjectList* l) {
  this->currentIndex = 0;
  this->list = l;
}

void ObjectListIterator::first() { this->currentIndex = 0; }

void ObjectListIterator::next() {
  this->currentIndex = min(this->currentIndex + 1, this->list->count);
}

auto ObjectListIterator::isDone() const -> bool {
  return this->currentIndex >= this->list->count;
}

auto ObjectListIterator::currentObject() const -> Object* {
  if (this->currentIndex < this->list->count) {
    return this->list->obj[this->currentIndex];
  }
  return nullptr;
}

}  // namespace df
