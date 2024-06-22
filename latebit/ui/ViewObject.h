#pragma once

#include <string>

#include "latebit/core/events/Event.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/objects/Object.h"

namespace lb {
enum ViewObjectLocation {
  TOP_LEFT,
  TOP_CENTER,
  TOP_RIGHT,
  CENTER_LEFT,
  CENTER_CENTER,
  CENTER_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_CENTER,
  BOTTOM_RIGHT
};

class ViewObject : public Object {
 private:
  // The label for the value (e.g., "Score")
  string label = "";
  // The view value (e.g., "12")
  int value = 0;
  // True if value should be drawn
  bool drawValue = true;
  // Color for text and value
  Color::Color color = COLOR_DEFAULT;
  // Location of view object on screen
  ViewObjectLocation location = ViewObjectLocation::TOP_CENTER;

  // Get view label and value as string
  [[nodiscard]] auto getDisplayString() -> string;

  // Update position and box depending on content and location
  auto refresh() -> void;

 public:
  ViewObject();

  // Draw view string and value
  auto draw() -> int override;
  // Handle "view" event if tag matches view string
  // Return 0 if ignored, else 1
  auto eventHandler(const Event* p_e) -> int override;

  // Set location of ViewObject on screen
  void setLocation(ViewObjectLocation l);
  // Get location of ViewObject on screen
  [[nodiscard]] auto getLocation() const -> ViewObjectLocation;

  // Set view value
  void setValue(int new_value);
  // Get view value
  [[nodiscard]] auto getValue() const -> int;

  // Set view color (for value, label and border)
  void setColor(Color::Color color);
  // Get view color (for value, label and border)
  [[nodiscard]] auto getColor() const -> Color::Color;

  // Set view label
  void setLabel(string label);
  // Get view label
  [[nodiscard]] auto getLabel() const -> string;

  // Set to true to draw value
  void setDrawValue(bool drawValue);
  // Returns true if value is drawn
  [[nodiscard]] auto getDrawValue() const -> bool;
};
}  // namespace lb