#pragma once

#include <utility>

#include "Event.h"

using namespace std;

namespace lb {
const string VIEW_EVENT = "lb::view";

class EventView : public Event {
 private:
  // The type of the ViewObject to update
  const string objectType = "";
  // Set to true to update the value by adding the new value to the old one
  const bool delta = false;
  // The new value to provide to the view object
  const int value = 0;

 public:
  EventView() : Event(VIEW_EVENT) {};
  EventView(string objectType, int value, bool delta = false)
    : Event(VIEW_EVENT),
      objectType(std::move(objectType)),
      delta(delta),
      value(value) {};

  // Get the type of the ViewObject to update
  [[nodiscard]] auto getObjectType() const -> string;

  // Get the new value to provide to the view object
  [[nodiscard]] auto getValue() const -> int;

  // Returns true to update the value by adding the new value to the old one
  [[nodiscard]] auto getDelta() const -> bool;
};
}  // namespace lb