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
  auto getId() const -> int;

  void setType(std::string t);
  std::string getType() const;

  void setPosition(Vector p);
  auto getPosition() const -> Vector;

  void setAltitude(int a);
  auto getAltitude() const -> int;

  void setDirection(Vector d);
  auto getDirection() const -> Vector;

  void setSpeed(float s);
  auto getSpeed() const -> float;

  void setVelocity(Vector v);
  auto getVelocity() const -> Vector;

  auto predictPosition() -> Vector;

  auto isSolid() const -> bool;
  void setSolidness(Solidness s);
  auto getSolidness() const -> Solidness;

  void setAnimation(Animation a);
  auto getAnimation() const -> Animation;

  void setDebug(bool debug);
  auto getDebug() const -> bool;

  auto setSprite(std::string label) -> int;

  void setBox(Box box);
  auto getBox() const -> Box;
  auto drawBoundingBox() const -> int;

  // Returns bounding box in world coordinates relative to this object's
  // position.
  auto getWorldBox() const -> Box;
  // Returns bounding box in world coordinates relative to center.
  auto getWorldBox(Vector center) const -> Box;

  virtual auto eventHandler(const Event *e) -> int;
  virtual auto draw() -> int;
};
}  // namespace df
