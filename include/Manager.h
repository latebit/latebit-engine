#pragma once

#include <string>

#include "Event.h"
#include "Manager.h"

namespace df {
class Manager {
 private:
  std::string type;
  bool is_started;

 protected:
  void setType(std::string type);

 public:
  Manager();
  virtual ~Manager();

  std::string getType() const;
  bool isStarted() const;

  virtual int startUp();
  virtual void shutDown();

  int onEvent(const Event *event) const;
};
};  // namespace df
