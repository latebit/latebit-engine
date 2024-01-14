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
int Object::getId() const { return this->id; }

void Object::setType(std::string t) { this->type = t; }
std::string Object::getType() const { return this->type; }

void Object::setPosition(Vector p) { this->position = p; }
Vector Object::getPosition() const { return this->position; }

void Object::setAltitude(int a) {
  if (a <= MAX_ALTITUDE && a >= 0) this->altitude = a;
}
int Object::getAltitude() const { return this->altitude; }

void Object::setDirection(Vector d) { this->direction = d; }
Vector Object::getDirection() const { return this->direction; }

void Object::setSpeed(float s) { this->speed = s; }
float Object::getSpeed() const { return this->speed; }

void Object::setVelocity(Vector v) {
  this->speed = v.getMagnitude();
  v.normalize();
  this->direction = v;
}
Vector Object::getVelocity() const {
  auto v = this->direction;
  v.scale(this->speed);
  return v;
}

Vector Object::predictPosition() { return this->position + getVelocity(); }

bool Object::isSolid() const { return this->solidness != SPECTRAL; }

void Object::setSolidness(Solidness s) { this->solidness = s; }
Solidness Object::getSolidness() const { return this->solidness; }

void Object::setAnimation(Animation a) { this->animation = a; }
Animation Object::getAnimation() const { return this->animation; }

int Object::setSprite(std::string label) {
  auto p_s = RM.getSprite(label);
  if (p_s == nullptr) return -1;

  this->animation.setSprite(p_s);
  setBox(this->animation.getBox());

  return 0;
}

void Object::setBox(Box box) { this->bounding_box = box; }
Box Object::getBox() const { return this->bounding_box; }

Box Object::getWorldBox() const { return getWorldBox(this->position); }
Box Object::getWorldBox(Vector center) const {
  auto corner = this->bounding_box.getCorner() + center;
  return Box(corner, this->bounding_box.getWidth(), this->bounding_box.getHeight());
}

int Object::eventHandler(const Event* p_e) { return 0; }

int Object::draw() {
  Vector p = getPosition();

  int result = this->animation.draw(p);
  if (this->debug) {
    result += drawBoundingBox();
  }

  return result;
}

void Object::setDebug(bool debug) { this->debug = debug; }
bool Object::getDebug() const { return this->debug; }

int Object::drawBoundingBox() const {
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
