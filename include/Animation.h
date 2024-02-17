#pragma once

#include <string>

#include "Box.h"
#include "Sprite.h"
#include "Vector.h"

using namespace std;

namespace lb {
int const STOP_ANIMATION_SLOWDOWN = -1;

class Animation {
 private:
  // Sprite to be animated
  Sprite *sprite;
  // Name of the animation
  string name;
  // Index of the current frame
  int index;
  // Number of frames to wait before changing frame
  int slowdownCount;

 public:
  Animation();

  // Set the sprite to be animated
  void setSprite(Sprite *s);
  // Get the sprite being animated
  [[nodiscard]] auto getSprite() const -> Sprite *;

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
  auto draw(Vector position) -> int;

  // Returns the bounding box of the current frame
  [[nodiscard]] auto getBox() const -> Box;

  auto operator==(const Animation &other) const -> bool;
};
}  // namespace lb
