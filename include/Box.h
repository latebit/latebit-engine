#pragma once

#include <string>

#include "Vector.h"

using namespace std;

namespace lb {
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

  // Create box with upper left corner at (0,0), width, and height
  Box(float width, float height);

  // Create box with upper left corner at topLeft, width, and height
  Box(Vector topLeft, float width, float height);

  // Set upper left corner of box
  void setCorner(Vector corner);
  // Get upper left corner of box
  [[nodiscard]] auto getCorner() const -> Vector;

  // Set width of box
  void setWidth(float width);
  // Get width of box
  [[nodiscard]] auto getWidth() const -> float;

  // Set height of box
  void setHeight(float height);
  // Get height of box
  [[nodiscard]] auto getHeight() const -> float;

  // Compare boxes, true if same
  auto operator==(const Box &other) const -> bool;
  // Represent the box as a string
  [[nodiscard]] auto toString() const -> string;
};
}  // namespace lb
