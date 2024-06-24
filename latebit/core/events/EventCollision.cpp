#include "EventCollision.h"

namespace lb {

auto EventCollision::getFirstObject() const -> const Object* {
  return this->firstObject;
}

auto EventCollision::getSecondObject() const -> const Object* {
  return this->secondObject;
}

auto EventCollision::getPosition() const -> const Vector {
  return this->position;
}

}  // namespace lb