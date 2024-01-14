#pragma once

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
  int id;
  std::string type;
  Vector position;
  int altitude;
  Vector direction;
  float speed;
  Solidness solidness;
  Animation animation;
  Box bounding_box;
  bool debug;

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

  void setDebug(bool debug);
  bool getDebug() const;

  int setSprite(std::string label);

  void setBox(Box box);
  Box getBox() const;
  int drawBoundingBox() const;

  // Returns bounding box in world coordinates relative to this object's
  // position.
  Box getWorldBox() const;
  // Returns bounding box in world coordinates relative to center.
  Box getWorldBox(Vector center) const;

  virtual int eventHandler(const Event *e);
  virtual int draw();
};
}  // namespace df
