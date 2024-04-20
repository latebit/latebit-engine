#pragma once

#include "Event.h"

using namespace std;

namespace lb {
const string VIEW_EVENT = "lb::view";

class EventView : public Event {
 private:
  // The type of the ViewObject to update
  string objectType;
  // Set to true to update the value by adding the new value to the old one
  bool delta;
  // The new value to provide to the view object
  int value;

 public:
  EventView();
  EventView(string objectType, int value, bool delta = false);

  // Set the type of the ViewObject to update
  void setObjectType(string type);
  // Get the type of the ViewObject to update
  [[nodiscard]] auto getObjectType() const -> string;

  // Set the new value to provide to the view object
  void setValue(int value);
  // Get the new value to provide to the view object
  [[nodiscard]] auto getValue() const -> int;

  // Set to true to update the value by adding the new value to the old one
  void setDelta(bool delta);
  // Returns true to update the value by adding the new value to the old one
  [[nodiscard]] auto getDelta() const -> bool;
};
}  // namespace lb