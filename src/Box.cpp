#include "Box.h"

#include <string>

#include "Vector.h"
#include "utils.h"

namespace df {
Box::Box() {
  this->toleft_corner = Vector();
  this->width = 0;
  this->height = 0;
}

Box::Box(Vector toleft, float horizontal, float vertical) {
  this->toleft_corner = toleft;
  this->width = horizontal;
  this->height = vertical;
}

void Box::setCorner(Vector corner) { this->toleft_corner = corner; }
auto Box::getCorner() const -> Vector { return this->toleft_corner; }

void Box::setWidth(float width) { this->width = width; }
auto Box::getWidth() const -> float { return this->width; }

void Box::setHeight(float height) { this->height = height; }
auto Box::getHeight() const -> float { return this->height; }

auto Box::operator==(const Box& other) const -> bool {
  return this->toleft_corner == other.toleft_corner &&
         equals(this->width, other.width) && equals(this->height, other.height);
}

auto Box::toString() const -> std::string {
  return "Box(" + this->toleft_corner.toString() + ", " +
         std::to_string(this->width) + ", " + std::to_string(this->height) +
         ")";
}
}  // namespace df