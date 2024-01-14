#include "Manager.h"

#include "Event.h"
#include "ObjectListIterator.h"
#include "WorldManager.h"

namespace df {

void Manager::setType(std::string type) { this->type = type; }

Manager::Manager() {
  this->is_started = false;
  this->type = "";
}

Manager::~Manager() {}

std::string Manager::getType() const { return this->type; }

bool Manager::isStarted() const { return this->is_started; }

int Manager::startUp() {
  this->is_started = true;
  return 0;
}

void Manager::shutDown() { this->is_started = false; }

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
