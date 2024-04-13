#pragma once
#include <string>

using namespace std;

namespace lb {
const string UNDEFINED_EVENT = "lb::undefined";

class Event {
 private:
  // Holds the type of the event
  string type = UNDEFINED_EVENT;

 public:
  Event();
  virtual ~Event();

  // Set the type of the event
  void setType(string t);
  // Get the type of the event
  [[nodiscard]] auto getType() const -> string;
};
}  // namespace lb
