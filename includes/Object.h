#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>

#include "Animation.h"
#include "Box.h"
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
  Animation m_animation;
  Box m_bounding_box;

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

  void setAnimation(Animation a);
  Animation getAnimation() const;

  int setSprite(std::string label);

  void setBox(Box box);
  Box getBox() const;
  // Returns bounding box in world coordinates relative to this object's
  // position.
  Box getWorldBox() const;
  // Returns bounding box in world coordinates relative to center.
  Box getWorldBox(Vector center) const;

  virtual int eventHandler(const Event *p_e);
  virtual int draw();
};
}  // namespace df

#endif