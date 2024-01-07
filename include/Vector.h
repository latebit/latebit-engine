#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <string>

namespace df {
class Vector {
 private:
  float m_x;
  float m_y;

 public:
  Vector(float x = 0.0f, float y = 0.0f);

  void setX(float x);
  float getX() const;

  void setY(float y);
  float getY() const;

  void setXY(float x = 0.0f, float y = 0.0f);

  float getMagnitude() const;

  void normalize();

  void scale(float a);

  float distance(const Vector *other);

  Vector operator+(const Vector &other) const;
  Vector operator-(const Vector &other) const;
  bool operator==(const Vector &other) const;
  bool operator!=(const Vector &other) const;
  bool operator!(void) const;

  std::string toString() const;
};

}  // namespace df
#endif