#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <string>

#include "Sprite.h"
#include "Vector.h"

namespace df {
int const STOP_ANIMATION_SLOWDOWN = -1;

class Animation {
 private:
  Sprite *m_p_sprite;
  std::string m_name;
  int m_index;
  int m_slowdown_count;

 public:
  Animation();

  void setSprite(Sprite *p_new_sprite);
  Sprite *getSprite() const;

  void setName(std::string new_name);
  std::string getName() const;

  void setIndex(int new_index);
  int getIndex() const;

  void setSlowdownCount(int new_slowdown_count);
  int getSlowdownCount() const;

  int draw(Vector position);

  bool operator==(const Animation &other) const;
};
}  // namespace df
#endif