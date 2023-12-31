#include "Object.h"

#include "WorldManager.h"

namespace df {

Object::Object() {
  static int id = 0;
  setId(id++);
  WM.insertObject(this);
}

Object::~Object() { WM.removeObject(this); }

void Object::setId(int id) { m_id = id; }

int Object::getId() const { return m_id; }

void Object::setType(std::string t) { m_type = t; }

std::string Object::getType() const { return m_type; }

void Object::setPosition(Vector p) { m_position = p; }

Vector Object::getPosition() const { return m_position; }

int Object::eventHandler(const Event* p_e) { return 0; }
}  // namespace df
