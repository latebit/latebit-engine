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
Vector Box::getCorner() const { return this->toleft_corner; }

void Box::setWidth(float width) { this->width = width; }
float Box::getWidth() const { return this->width; }

void Box::setHeight(float height) { this->height = height; }
float Box::getHeight() const { return this->height; }

bool Box::operator==(const Box& other) const {
  return this->toleft_corner == other.toleft_corner &&
         equals(this->width, other.width) && equals(this->height, other.height);
}

std::string Box::toString() const {
  return "Box(" + this->toleft_corner.toString() + ", " +
         std::to_string(this->width) + ", " + std::to_string(this->height) +
         ")";
}
}  // namespace df