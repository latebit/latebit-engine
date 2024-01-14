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
  auto isStarted() const -> bool;

  virtual auto startUp() -> int;
  virtual void shutDown();

  auto onEvent(const Event *event) const -> int;
};
};  // namespace df
