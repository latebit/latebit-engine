#include "ObjectList.h"

namespace df {

ObjectList::ObjectList() { this->count = 0; }

int ObjectList::insert(Object* o) {
  if (isFull()) return -1;

  this->obj[this->count] = o;
  this->count++;
  return 0;
}

int ObjectList::remove(Object* o) {
  for (int i = 0; i < this->count; i++) {
    if (this->obj[i] == o) {
      for (int j = i; j < this->count - 1; j++) {
        this->obj[j] = this->obj[j + 1];
      }
      this->count--;
      return 0;
    }
  }

  return -1;
}
void ObjectList::clear() { this->count = 0; }
int ObjectList::getCount() const { return this->count; }
bool ObjectList::isEmpty() const { return this->count == 0; }
bool ObjectList::isFull() const { return this->count != 0 && this->count >= MAX_SIZE; }
int ObjectList::find(Object* o) const {
  for (int i = 0; i < this->count; i++) {
    if (this->obj[i] == o) {
      return i;
    }
  }

  return -1;
}
}  // namespace df