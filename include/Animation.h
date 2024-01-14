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
  Sprite *getSprite() const;

  void setName(std::string new_name);
  std::string getName() const;

  void setIndex(int new_index);
  int getIndex() const;

  void setSlowdownCount(int new_slowdown_count);
  int getSlowdownCount() const;

  int draw(Vector position);

  Box getBox() const;

  bool operator==(const Animation &other) const;
};
}  // namespace df
