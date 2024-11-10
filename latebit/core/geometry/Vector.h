#pragma once

#include <string>

namespace lb {
class Vector {
 private:
  // The horizontal component of the vector
  float x = 0;
  // The vertical component of the vector
  float y = 0;

 public:
  Vector(float x = 0.0f, float y = 0.0f);

  // Set the horizontal component of the vector
  void setX(float x);
  // Get the horizontal component of the vector
  [[nodiscard]] auto getX() const -> float;

  // Set the vertical component of the vector
  void setY(float y);
  // Get the vertical component of the vector
  [[nodiscard]] auto getY() const -> float;

  // Set both components of the vector
  void setXY(float x = 0.0f, float y = 0.0f);

  // Returns the length of the vector
  [[nodiscard]] auto getMagnitude() const -> float;

  // Returns a new vector that is the normalized version (magnitude 1) of this
  // vector
  [[nodiscard]] auto normalize() const -> Vector;

  // Returns the distance between this vector and another
  auto distance(const Vector &other) -> float;

  // Returns the dot product of this vector and another
  auto dot(const Vector &other) -> float;
  // Returns a new vector that is the sum of two vectors
  auto operator+(const Vector &other) const -> Vector;
  // Returns a new vector that is the difference of two vectors
  auto operator-(const Vector &other) const -> Vector;
  // Returns a new vector scaled by a value
  auto operator*(const float value) const -> Vector;

  // Returns true if two vectors are the same
  auto operator==(const Vector &other) const -> bool;
  // Returns true if two vectors are not the same
  auto operator!=(const Vector &other) const -> bool;
  // Returns true if the vector is the zero vector
  auto operator!() const -> bool;
  // Returns a string representation of the vector
  [[nodiscard]] auto toString() const -> std::string;
};

}  // namespace lb
