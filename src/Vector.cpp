#include "Vector.h"

#include <cmath>
#include <cstdio>
#include <string>

#include "utils.h"

namespace df {
Vector::Vector(float x, float y) {
  this->x = x;
  this->y = y;
}

void Vector::setX(float x) { this->x = x; }
float Vector::getX() const { return this->x; }

void Vector::setY(float y) { this->y = y; }
float Vector::getY() const { return this->y; }

void Vector::setXY(float x, float y) {
  this->x = x;
  this->y = y;
}

float Vector::getMagnitude() const {
  return std::sqrt(this->x * this->x + this->y * this->y);
}

void Vector::scale(float a) {
  this->x *= a;
  this->y *= a;
}

float Vector::distance(const Vector* other) {
  return (*this - *other).getMagnitude();
}

void Vector::normalize() {
  float length = getMagnitude();
  if (length > 0) {
    this->x = this->x / length;
    this->y = this->y / length;
  }
}

Vector Vector::operator+(const Vector& other) const {
  return Vector(this->x + other.x, this->y + other.y);
}

Vector Vector::operator-(const Vector& other) const {
  return Vector(this->x - other.x, this->y - other.y);
}

bool Vector::operator==(const Vector& other) const {
  return equals(this->x, other.x) && equals(this->y, other.y);
}

bool Vector::operator!=(const Vector& other) const {
  return !equals(this->x, other.x) || !equals(this->y, other.y);
}

bool Vector::operator!() const { return this->x == 0 && this->y == 0; }

std::string Vector::toString() const {
  return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")";
}

}  // namespace df