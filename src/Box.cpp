#include "Box.h"

#include <string>

#include "Vector.h"
#include "utils.h"

namespace lb {
Box::Box() {
  this->corner = Vector();
  this->width = 0;
  this->height = 0;
}

Box::Box(Vector topLeft, float horizontal, float vertical) {
  this->corner = topLeft;
  this->width = horizontal;
  this->height = vertical;
}

Box::Box(float horizontal, float vertical) {
  this->corner = Vector();
  this->width = horizontal;
  this->height = vertical;
}

void Box::setCorner(Vector corner) { this->corner = corner; }
auto Box::getCorner() const -> Vector { return this->corner; }

void Box::setWidth(float width) { this->width = width; }
auto Box::getWidth() const -> float { return this->width; }

void Box::setHeight(float height) { this->height = height; }
auto Box::getHeight() const -> float { return this->height; }

auto Box::operator==(const Box& other) const -> bool {
  return this->corner == other.corner && equals(this->width, other.width) &&
         equals(this->height, other.height);
}

auto Box::toString() const -> std::string {
  return "Box(" + this->corner.toString() + ", " + std::to_string(this->width) +
         ", " + std::to_string(this->height) + ")";
}
}  // namespace lb