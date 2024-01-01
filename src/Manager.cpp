#include "Manager.h"

#include "Event.h"
#include "ObjectListIterator.h"
#include "WorldManager.h"

namespace df {

void Manager::setType(std::string type) { m_type = type; }

Manager::Manager() {
  m_is_started = false;
  m_type = "";
}

Manager::~Manager() {}

std::string Manager::getType() const { return m_type; }

bool Manager::isStarted() const { return m_is_started; }

int Manager::startUp() {
  m_is_started = true;
  return 0;
}

void Manager::shutDown() { m_is_started = false; }

int Manager::onEvent(const Event* p_event) const {
  int count = 0;

  auto objects = WM.getAllObjects();
  auto iterator = ObjectListIterator(&objects);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    iterator.currentObject()->eventHandler(p_event);
    count++;
  }

  return count;
}

}  // namespace df
