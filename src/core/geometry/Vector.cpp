#include "Vector.h"

#include <cmath>
#include <string>

#include "core/utils/utils.h"

namespace lb {
Vector::Vector(float x, float y) {
  this->x = x;
  this->y = y;
}

void Vector::setX(float x) { this->x = x; }
auto Vector::getX() const -> float { return this->x; }

void Vector::setY(float y) { this->y = y; }
auto Vector::getY() const -> float { return this->y; }

void Vector::setXY(float x, float y) {
  this->x = x;
  this->y = y;
}

auto Vector::getMagnitude() const -> float {
  return std::sqrt(this->x * this->x + this->y * this->y);
}

void Vector::scale(float a) {
  this->x *= a;
  this->y *= a;
}

auto Vector::distance(const Vector* other) -> float {
  return (*this - *other).getMagnitude();
}

void Vector::normalize() {
  float length = getMagnitude();
  if (length > 0) {
    this->x = this->x / length;
    this->y = this->y / length;
  }
}

auto Vector::operator+(const Vector& other) const -> Vector {
  return {this->x + other.x, this->y + other.y};
}

auto Vector::operator-(const Vector& other) const -> Vector {
  return {this->x - other.x, this->y - other.y};
}

auto Vector::operator==(const Vector& other) const -> bool {
  return equals(this->x, other.x) && equals(this->y, other.y);
}

auto Vector::operator!=(const Vector& other) const -> bool {
  return !equals(this->x, other.x) || !equals(this->y, other.y);
}

auto Vector::operator!() const -> bool { return this->x == 0 && this->y == 0; }

auto Vector::toString() const -> std::string {
  return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")";
}

}  // namespace lb