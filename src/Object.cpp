#include "Object.h"

#include "DisplayManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

namespace df {

Object::Object() {
  static int id = 0;
  setId(id++);
  m_type = "Object";
  m_position = Vector();
  m_altitude = MAX_ALTITUDE / 2;
  m_solidness = HARD;
  m_direction = Vector();
  m_animation = Animation();
  m_bounding_box = Box(m_position, 1, 1);
  m_speed = 0;
  m_debug = false;
  WM.insertObject(this);
}

Object::~Object() {
  // Resources for this object are freed in WorldManager::shutDown
  WM.removeObject(this);
}

void Object::setId(int id) { m_id = id; }
int Object::getId() const { return m_id; }

void Object::setType(std::string t) { m_type = t; }
std::string Object::getType() const { return m_type; }

void Object::setPosition(Vector p) { m_position = p; }
Vector Object::getPosition() const { return m_position; }

void Object::setAltitude(int a) {
  if (a <= MAX_ALTITUDE && a >= 0) m_altitude = a;
}
int Object::getAltitude() const { return m_altitude; }

void Object::setDirection(Vector d) { m_direction = d; }
Vector Object::getDirection() const { return m_direction; }

void Object::setSpeed(float s) { m_speed = s; }
float Object::getSpeed() const { return m_speed; }

void Object::setVelocity(Vector v) {
  m_speed = v.getMagnitude();
  v.normalize();
  m_direction = v;
}
Vector Object::getVelocity() const {
  auto v = m_direction;
  v.scale(m_speed);
  return v;
}

Vector Object::predictPosition() { return m_position + getVelocity(); }

bool Object::isSolid() const { return m_solidness != SPECTRAL; }

void Object::setSolidness(Solidness s) { m_solidness = s; }
Solidness Object::getSolidness() const { return m_solidness; }

void Object::setAnimation(Animation a) { m_animation = a; }
Animation Object::getAnimation() const { return m_animation; }

int Object::setSprite(std::string label) {
  auto p_s = RM.getSprite(label);
  if (p_s == nullptr) return -1;

  m_animation.setSprite(p_s);
  setBox(m_animation.getBox());

  return 0;
}

void Object::setBox(Box box) { m_bounding_box = box; }
Box Object::getBox() const { return m_bounding_box; }

Box Object::getWorldBox() const { return getWorldBox(m_position); }
Box Object::getWorldBox(Vector center) const {
  auto corner = m_bounding_box.getCorner() + center;
  return Box(corner, m_bounding_box.getWidth(), m_bounding_box.getHeight());
}

int Object::eventHandler(const Event* p_e) { return 0; }

int Object::draw() {
  Vector p = getPosition();

  int result = m_animation.draw(p);
  if (m_debug) {
    result += drawBoundingBox();
  }

  return result;
}

void Object::setDebug(bool debug) { m_debug = debug; }
bool Object::getDebug() const { return m_debug; }

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
