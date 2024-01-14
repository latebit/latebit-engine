#pragma once

#include <string>

#include "Box.h"
#include "Sprite.h"
#include "Vector.h"

namespace df {
int const STOANIMATION_SLOWDOWN = -1;

class Animation {
 private:
  Sprite *sprite;
  std::string name;
  int index;
  int slowdownCount;

 public:
  Animation();

  void setSprite(Sprite *new_sprite);
  auto getSprite() const -> Sprite *;

  void setName(std::string new_name);
  std::string getName() const;

  void setIndex(int new_index);
  auto getIndex() const -> int;

  void setSlowdownCount(int new_slowdown_count);
  auto getSlowdownCount() const -> int;

  auto draw(Vector position) -> int;

  auto getBox() const -> Box;

  auto operator==(const Animation &other) const -> bool;
};
}  // namespace df
