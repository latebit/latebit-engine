#include "ObjectUtils.h"

#include <algorithm>

namespace lb {

void remove(vector<Object*>& l, Object* o) {
  l.erase(
    std::remove_if(l.begin(), l.end(), [o](const Object* p) { return p == o; }),
    l.end());
}

auto contains(const vector<unique_ptr<Object>>& l, Object* o) -> bool {
  return std::find_if(l.begin(), l.end(), [o](const unique_ptr<Object>& p) {
           return p.get() == o;
         }) != l.end();
}

auto contains(vector<Object*> l, Object* o) -> bool {
  return std::find(l.begin(), l.end(), o) != l.end();
}

void insert(vector<Object*>& l, Object* o) {
  if (contains(l, o)) return;

  l.push_back(o);
  if (l.size() == l.capacity()) {
    l.reserve(l.size() + CHUNK_SIZE);
  }
}

void insert(vector<unique_ptr<Object>>& l, unique_ptr<Object> o) {
  l.push_back(std::move(o));
  if (l.size() == l.capacity()) {
    l.reserve(l.size() + CHUNK_SIZE);
  }
}

}  // namespace lb