#pragma once

#include <string>

#include "Vector.h"

namespace df {
class Box {
 private:
  // Bottom left corner of box
  Vector corner;
  // Width of box
  float width;
  // Height of box
  float height;

 public:
  Box();
  Box(float width, float height);
  Box(Vector topLeft, float width, float height);

  void setCorner(Vector corner);
  [[nodiscard]] auto getCorner() const -> Vector;

  void setWidth(float width);
  [[nodiscard]] auto getWidth() const -> float;

  void setHeight(float height);
  [[nodiscard]] auto getHeight() const -> float;

  auto operator==(const Box &other) const -> bool;
  [[nodiscard]] auto toString() const -> std::string;
};
}  // namespace df
