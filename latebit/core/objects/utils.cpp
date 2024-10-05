#include "utils.h"
#include <algorithm>

namespace lb {

void remove(vector<Object*>& l, Object* o) {
  l.erase(std::remove_if(
    l.begin(), l.end(),
    [o](const Object*p) { return p == o; }),
    l.end());
}

auto contains(const std::vector<std::unique_ptr<Object>>& l, Object* o) -> bool {
  return std::find_if(l.begin(), l.end(), [o](const std::unique_ptr<Object>& p) { return p.get() == o; }) != l.end();
}

auto contains(vector<Object*> l, Object* o) -> bool {
  return std::find(l.begin(), l.end(), o) != l.end();
}

}