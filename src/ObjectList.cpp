#include "ObjectList.h"

namespace df {

ObjectList::ObjectList() { this->count = 0; }

int ObjectList::insert(Object* p_o) {
  if (isFull()) return -1;

  this->p_obj[this->count] = p_o;
  this->count++;
  return 0;
}

int ObjectList::remove(Object* p_o) {
  for (int i = 0; i < this->count; i++) {
    if (this->p_obj[i] == p_o) {
      for (int j = i; j < this->count - 1; j++) {
        this->p_obj[j] = this->p_obj[j + 1];
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
int ObjectList::find(Object* p_o) const {
  for (int i = 0; i < this->count; i++) {
    if (this->p_obj[i] == p_o) {
      return i;
    }
  }

  return -1;
}
}  // namespace df