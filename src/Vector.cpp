#include "Vector.h"

#include <stdio.h>

#include <cmath>
#include <string>

namespace df {
Vector::Vector(float x, float y) {
  m_x = x;
  m_y = y;
}

void Vector::setX(float x) { m_x = x; }
float Vector::getX() const { return m_x; }

void Vector::setY(float y) { m_y = y; }
float Vector::getY() const { return m_y; }

void Vector::setXY(float x, float y) {
  m_x = x;
  m_y = y;
}

float Vector::getMagnitude() const { return std::sqrt(m_x * m_x + m_y * m_y); }

void Vector::scale(float a) {
  m_x *= a;
  m_y *= a;
}

float Vector::distance(const Vector* other) {
  return (*this - *other).getMagnitude();
}

void Vector::normalize() {
  float length = getMagnitude();
  if (length > 0) {
    m_x = m_x / length;
    m_y = m_y / length;
  }
}

Vector Vector::operator+(const Vector& other) const {
  return Vector(m_x + other.m_x, m_y + other.m_y);
}

Vector Vector::operator-(const Vector& other) const {
  return Vector(m_x - other.m_x, m_y - other.m_y);
}

bool Vector::operator==(const Vector& other) const {
  return m_x == other.m_x && m_y == other.m_y;
}

bool Vector::operator!=(const Vector& other) const {
  return m_x != other.m_x || m_y != other.m_y;
}

bool Vector::operator!() const { return m_x == 0 && m_y == 0; }

std::string Vector::toString() const {
  return "(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")";
}

}  // namespace df