#include "Box.h"

#include "Vector.h"

namespace df {
Box::Box() {
  m_top_left_corner = Vector();
  m_width = 0;
  m_height = 0;
}

Box::Box(Vector top_left, float horizontal, float vertical) {
  m_top_left_corner = top_left;
  m_width = horizontal;
  m_height = vertical;
}

void Box::setCorner(Vector corner) { m_top_left_corner = corner; }
Vector Box::getCorner() const { return m_top_left_corner; }

void Box::setWidth(float width) { m_width = width; }
float Box::getWidth() const { return m_width; }

void Box::setHeight(float height) { m_height = height; }
float Box::getHeight() const { return m_height; }
}  // namespace df