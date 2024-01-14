#pragma once

#include <string>

#include "Vector.h"

namespace df {
class Box {
 private:
  Vector toleft_corner;
  float width;
  float height;

 public:
  Box();
  Box(Vector toleft, float width, float height);

  void setCorner(Vector corner);
  auto getCorner() const -> Vector;

  void setWidth(float width);
  auto getWidth() const -> float;

  void setHeight(float height);
  auto getHeight() const -> float;

  auto operator==(const Box &other) const -> bool;
  std::string toString() const;
};
}  // namespace df
