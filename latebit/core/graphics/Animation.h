#pragma once

#include <string>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Sprite.h"

using namespace std;

namespace lb {
int const STOP_ANIMATION_SLOWDOWN = -1;

class Animation {
 private:
  // Sprite to be animated
  const Sprite *sprite = nullptr;
  // Name of the animation
  string name = "";
  // Index of the current frame
  int index = 0;
  // Number of frames to wait before changing frame
  int slowdownCount = 0;

 public:
  Animation();

  // Set the sprite to be animated
  void setSprite(const Sprite *s);
  // Get the sprite being animated
  [[nodiscard]] auto getSprite() const -> const Sprite *;

  // Set the name of the animation
  void setName(string n);
  // Get the name of the animation
  [[nodiscard]] auto getName() const -> string;

  // Set the index of the current frame
  void setIndex(int i);
  // Get the index of the current frame
  [[nodiscard]] auto getIndex() const -> int;

  // Set the number of frames to wait before changing frame
  void setSlowdownCount(int c);
  // Get the number of frames to wait before changing frame
  [[nodiscard]] auto getSlowdownCount() const -> int;

  // Draws the current frame of the animation at the given position
  auto draw(Vector position, uint8_t scale = 1) -> int;

  auto operator==(const Animation &other) const -> bool;
};
}  // namespace lb
