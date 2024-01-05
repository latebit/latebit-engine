#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>

#include "Event.h"
#include "Vector.h"

namespace df {
enum Solidness {
  HARD,
  SOFT,
  SPECTRAL,
};

class Object {
 private:
  int m_id;
  std::string m_type;
  Vector m_position;
  int m_altitude;
  Vector m_direction;
  float m_speed;
  Solidness m_solidness;

 public:
  Object();

  virtual ~Object();

  void setId(int id);
  int getId() const;

  void setType(std::string t);
  std::string getType() const;

  void setPosition(Vector p);
  Vector getPosition() const;

  void setAltitude(int a);
  int getAltitude() const;

  void setDirection(Vector d);
  Vector getDirection() const;

  void setSpeed(float s);
  float getSpeed() const;

  void setVelocity(Vector v);
  Vector getVelocity() const;

  Vector predictPosition();

  bool isSolid() const;
  void setSolidness(Solidness s);
  Solidness getSolidness() const;

  virtual int eventHandler(const Event *p_e);
  virtual int draw();
};
}  // namespace df

#endif