#pragma once

#include <string>

namespace df {
class Vector {
 private:
  float x;
  float y;

 public:
  Vector(float x = 0.0f, float y = 0.0f);

  void setX(float x);
  [[nodiscard]] auto getX() const -> float;

  void setY(float y);
  [[nodiscard]] auto getY() const -> float;

  void setXY(float x = 0.0f, float y = 0.0f);

  [[nodiscard]] auto getMagnitude() const -> float;

  void normalize();

  void scale(float a);

  auto distance(const Vector *other) -> float;

  auto operator+(const Vector &other) const -> Vector;
  auto operator-(const Vector &other) const -> Vector;
  auto operator==(const Vector &other) const -> bool;
  auto operator!=(const Vector &other) const -> bool;
  auto operator!() const -> bool;

  [[nodiscard]] auto toString() const -> std::string;
};

}  // namespace df
