#pragma once
#include <string>

using namespace std;

namespace lb {
const string UNDEFINED_EVENT = "lb::undefined";

class Event {
 private:
  // Holds the type of the event
  const string type;

 public:
  Event(const string& type = UNDEFINED_EVENT) : type(type){};
  virtual ~Event() = default;

  // Get the type of the event
  [[nodiscard]] auto getType() const -> string;
};
}  // namespace lb
