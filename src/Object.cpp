#include "Object.h"

#include "DisplayManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneGraph.h"
#include "WorldManager.h"

using namespace std;

namespace df {

Object::Object() {
  static int id = 0;
  this->id = id++;
  WM.insertObject(this);
}

Object::~Object() {
  // Resources for this object are freed in WorldManager::shutDown/update
  WM.removeObject(this);
}

auto Object::getId() const -> int { return this->id; }

void Object::setType(string t) { this->type = t; }
auto Object::getType() const -> string { return this->type; }

void Object::setPosition(Vector p) { this->position = p; }
auto Object::getPosition() const -> Vector { return this->position; }

void Object::setAltitude(int a) {
  if (a <= MAX_ALTITUDE && a >= 0) {
    WM.getSceneGraph().setAltitude(this, a);
    this->altitude = a;
  }
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

auto Object::predictPosition() -> Vector {
  return this->position + getVelocity();
}

auto Object::isSolid() const -> bool { return this->solidness != SPECTRAL; }

void Object::setSolidness(Solidness s) {
  WM.getSceneGraph().setSolidness(this, s);
  this->solidness = s;
}
auto Object::getSolidness() const -> Solidness { return this->solidness; }

void Object::setAnimation(Animation a) { this->animation = a; }
auto Object::getAnimation() const -> Animation { return this->animation; }

auto Object::setSprite(string label) -> int {
  auto s = RM.getSprite(label);
  if (s == nullptr) return -1;

  this->animation.setSprite(s);
  setBox(this->animation.getBox());

  return 0;
}

void Object::setBox(Box box) { this->boundingBox = box; }
auto Object::getBox() const -> Box { return this->boundingBox; }

auto Object::getWorldBox() const -> Box { return getWorldBox(this->position); }
auto Object::getWorldBox(Vector center) const -> Box {
  auto boundingBox = this->boundingBox;
  auto corner = boundingBox.getCorner() + center;
  return {corner, boundingBox.getWidth(), boundingBox.getHeight()};
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

auto Object::subscribe(string eventType) -> int {
  if (WM.isValid(eventType)) {
    return WM.subscribe(this, eventType);
  } else if (IM.isValid(eventType)) {
    return IM.subscribe(this, eventType);
  } else if (GM.isValid(eventType)) {
    return GM.subscribe(this, eventType);
  }
  return -1;
}

auto Object::setActive(bool active) -> void {
  WM.getSceneGraph().setActive(this, active);
  this->active = active;
}
auto Object::isActive() const -> bool { return this->active; }

auto Object::setVisible(bool visible) -> void {
  WM.getSceneGraph().setVisible(this, visible);
  this->visible = visible;
}
auto Object::isVisible() const -> bool { return this->visible; }

auto Object::toString() const -> string {
  return "Object(id=" + to_string(this->id) + ",type=" + this->type + ")";
}

}  // namespace df
