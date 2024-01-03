#include "Object.h"

#include "WorldManager.h"

namespace df {

Object::Object() {
  static int id = 0;
  setId(id++);
  m_type = "Object";
  m_position = Vector();
  m_altitude = MAX_ALTITUDE / 2;
  WM.insertObject(this);
}

Object::~Object() { WM.removeObject(this); }

void Object::setId(int id) { m_id = id; }
int Object::getId() const { return m_id; }

void Object::setType(std::string t) { m_type = t; }
std::string Object::getType() const { return m_type; }

void Object::setPosition(Vector p) { m_position = p; }
Vector Object::getPosition() const { return m_position; }

void Object::setAltitude(int a) {
  if (a < MAX_ALTITUDE && a >= 0) m_altitude = a;
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

int Object::eventHandler(const Event* p_e) { return 0; }

int Object::draw() { return 0; }
}  // namespace df
