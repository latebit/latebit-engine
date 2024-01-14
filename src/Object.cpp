#include "Object.h"

#include "DisplayManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

namespace df {

Object::Object() {
  static int id = 0;
  setId(id++);
  this->type = "Object";
  this->position = Vector();
  this->altitude = MAX_ALTITUDE / 2;
  this->solidness = HARD;
  this->direction = Vector();
  this->animation = Animation();
  this->bounding_box = Box(this->position, 1, 1);
  this->speed = 0;
  this->debug = false;
  WM.insertObject(this);
}

Object::~Object() {
  // Resources for this object are freed in WorldManager::shutDown
  WM.removeObject(this);
}

void Object::setId(int id) { this->id = id; }
auto Object::getId() const -> int { return this->id; }

void Object::setType(std::string t) { this->type = t; }
auto Object::getType() const -> std::string { return this->type; }

void Object::setPosition(Vector p) { this->position = p; }
auto Object::getPosition() const -> Vector { return this->position; }

void Object::setAltitude(int a) {
  if (a <= MAX_ALTITUDE && a >= 0) this->altitude = a;
}
auto Object::getAltitude() const -> int { return this->altitude; }

void Object::setDirection(Vector d) { this->direction = d; }
auto Object::getDirection() const -> Vector { return this->direction; }

void Object::setSpeed(float s) { this->speed = s; }
auto Object::getSpeed() const -> float { return this->speed; }

void Object::setVelocity(Vector v) {
  this->speed = v.getMagnitude();
  v.normalize();
  this->direction = v;
}
auto Object::getVelocity() const -> Vector {
  auto v = this->direction;
  v.scale(this->speed);
  return v;
}

auto Object::predictPosition() -> Vector { return this->position + getVelocity(); }

auto Object::isSolid() const -> bool { return this->solidness != SPECTRAL; }

void Object::setSolidness(Solidness s) { this->solidness = s; }
auto Object::getSolidness() const -> Solidness { return this->solidness; }

void Object::setAnimation(Animation a) { this->animation = a; }
auto Object::getAnimation() const -> Animation { return this->animation; }

auto Object::setSprite(std::string label) -> int {
  auto s = RM.getSprite(label);
  if (s == nullptr) return -1;

  this->animation.setSprite(s);
  setBox(this->animation.getBox());

  return 0;
}

void Object::setBox(Box box) { this->bounding_box = box; }
auto Object::getBox() const -> Box { return this->bounding_box; }

auto Object::getWorldBox() const -> Box { return getWorldBox(this->position); }
auto Object::getWorldBox(Vector center) const -> Box {
  auto corner = this->bounding_box.getCorner() + center;
  return {corner, this->bounding_box.getWidth(),
             this->bounding_box.getHeight()};
}

auto Object::eventHandler(const Event* e) -> int { return 0; }

auto Object::draw() -> int {
  Vector p = getPosition();

  int result = this->animation.draw(p);
  if (this->debug) {
    result += drawBoundingBox();
  }

  return result;
}

void Object::setDebug(bool debug) { this->debug = debug; }
auto Object::getDebug() const -> bool { return this->debug; }

auto Object::drawBoundingBox() const -> int {
  Box box = getWorldBox();
  Vector corner = box.getCorner();
  float width = box.getWidth();
  float height = box.getHeight();

  int result = 0;
  result += DM.drawCh(corner, '+', CYAN);
  result += DM.drawCh(corner + df::Vector(width, 0), '+', CYAN);
  result += DM.drawCh(corner + df::Vector(width, height), '+', CYAN);
  result += DM.drawCh(corner + df::Vector(0, height), '+', CYAN);
  return result;
}
}  // namespace df
