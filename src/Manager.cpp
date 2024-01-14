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

Manager::~Manager() = default;

auto Manager::getType() const -> std::string { return this->type; }

auto Manager::isStarted() const -> bool { return this->is_started; }

auto Manager::startUp() -> int {
  this->is_started = true;
  return 0;
}

void Manager::shutDown() { this->is_started = false; }

auto Manager::onEvent(const Event* event) const -> int {
  int count = 0;

  auto objects = WM.getAllObjects();
  auto iterator = ObjectListIterator(&objects);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    iterator.currentObject()->eventHandler(event);
    count++;
  }

  return count;
}

}  // namespace df
