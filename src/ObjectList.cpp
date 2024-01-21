#include "ObjectList.h"

namespace df {

ObjectList::ObjectList() = default;

auto ObjectList::insert(Object* o) -> int {
  if (isFull()) return -1;

  this->obj[this->count] = o;
  this->count++;
  return 0;
}

auto ObjectList::remove(Object* o) -> int {
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
auto ObjectList::getCount() const -> int { return this->count; }
auto ObjectList::isEmpty() const -> bool { return this->count == 0; }
auto ObjectList::isFull() const -> bool {
  return this->count != 0 && this->count >= MAX_SIZE;
}
auto ObjectList::find(Object* o) const -> int {
  for (int i = 0; i < this->count; i++) {
    if (this->obj[i] == o) {
      return i;
    }
  }

  return -1;
}
}  // namespace df