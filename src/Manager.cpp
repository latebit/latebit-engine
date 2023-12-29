#include "Manager.h"

namespace df {

void Manager::setType(std::string type) { m_type = type; }

Manager::Manager() {}

Manager::~Manager() {}

std::string Manager::getType() const { return m_type; }

bool Manager::isStarted() const { return false; }

int Manager::startUp() { return 0; }

void Manager::shutDown() {}

}  // namespace df
