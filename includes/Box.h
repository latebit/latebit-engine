#ifndef __BOX_H__
#define __BOX_H__

#include "Vector.h"

namespace df {
class Box {
 private:
  Vector m_top_left_corner;
  float m_width;
  float m_height;

 public:
  Box();
  Box(Vector top_left, float width, float height);

  void setCorner(Vector corner);
  Vector getCorner() const;

  void setWidth(float width);
  float getWidth() const;

  void setHeight(float height);
  float getHeight() const;

  bool operator==(const Box &other) const;
};
}  // namespace df

#endif