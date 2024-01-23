#pragma once

#include <string>

#include "Box.h"
#include "Sprite.h"
#include "Vector.h"

namespace df {
int const STOP_ANIMATION_SLOWDOWN = -1;

class Animation {
 private:
  Sprite *sprite;
  std::string name;
  int index;
  int slowdownCount;

 public:
  Animation();

  void setSprite(Sprite *s);
  [[nodiscard]] auto getSprite() const -> Sprite *;

  void setName(std::string n);
  [[nodiscard]] auto getName() const -> std::string;

  void setIndex(int i);
  [[nodiscard]] auto getIndex() const -> int;

  void setSlowdownCount(int c);
  [[nodiscard]] auto getSlowdownCount() const -> int;

  auto draw(Vector position) -> int;

  [[nodiscard]] auto getBox() const -> Box;

  auto operator==(const Animation &other) const -> bool;
};
}  // namespace df
