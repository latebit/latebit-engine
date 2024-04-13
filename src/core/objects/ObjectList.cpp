#include "ObjectList.h"

#include "core/utils/Logger.h"

namespace lb {

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

auto ObjectList::operator+(ObjectList const& other) const -> ObjectList {
  ObjectList result = ObjectList();

  if (this->count + other.getCount() > MAX_SIZE) {
    Log.debug(
      "ObjectList::operator+: Resulting ObjectList is too large (got %d, max "
      "%d)",
      this->count + other.getCount(), MAX_SIZE);
    return result;
  }

  for (int i = 0; i < this->count; i++) {
    result.insert(this->obj[i]);
  }

  for (int i = 0; i < other.count; i++) {
    result.insert(other.obj[i]);
  }

  return result;
}

}  // namespace lb