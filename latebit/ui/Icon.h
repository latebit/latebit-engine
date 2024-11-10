#pragma once

#include <utility>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/graphics/Keyframe.h"

using namespace lb;

namespace lbui {
class Icon {
 private:
  const Keyframe frame = {};
  const int width = 1;
  const int height = 1;
  const int scaling = 1;

 public:
  Icon(Keyframe frame, int width, int height, int scaling = 1)
    : frame(std::move(frame)), width(width), height(height), scaling(scaling) {}

  [[nodiscard]] auto draw(Vector position) const -> int {
    return DM.drawKeyframe(position, &frame, width, height, scaling);
  }

  [[nodiscard]] auto getHeight() const -> int { return height; }
  [[nodiscard]] auto getWidth() const -> int { return width; }
  [[nodiscard]] auto getScaling() const -> int { return scaling; }
};
}  // namespace lbui