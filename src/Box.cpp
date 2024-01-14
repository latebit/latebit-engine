#include "Box.h"

#include <string>

#include "Vector.h"
#include "utils.h"

namespace df {
Box::Box() {
  this->top_left_corner = Vector();
  this->width = 0;
  this->height = 0;
}

Box::Box(Vector top_left, float horizontal, float vertical) {
  this->top_left_corner = top_left;
  this->width = horizontal;
  this->height = vertical;
}

void Box::setCorner(Vector corner) { this->top_left_corner = corner; }
Vector Box::getCorner() const { return this->top_left_corner; }

void Box::setWidth(float width) { this->width = width; }
float Box::getWidth() const { return this->width; }

void Box::setHeight(float height) { this->height = height; }
float Box::getHeight() const { return this->height; }

bool Box::operator==(const Box& other) const {
  return this->top_left_corner == other.top_left_corner &&
         equals(this->width, other.width) && equals(this->height, other.height);
}

std::string Box::toString() const {
  return "Box(" + this->top_left_corner.toString() + ", " +
         std::to_string(this->width) + ", " + std::to_string(this->height) +
         ")";
}
}  // namespace df