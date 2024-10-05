#pragma once

#include <memory>
#include "Object.h"

namespace lb {

void remove(vector<Object*>& l, Object* o);
auto contains(vector<Object*> l, Object* o) -> bool;
auto contains(const std::vector<std::unique_ptr<Object>>& l, Object* o) -> bool;

}