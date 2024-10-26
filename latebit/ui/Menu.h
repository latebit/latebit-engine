#pragma once

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/world/Object.h"
#include "latebit/ui/Control.h"

using namespace lb;

namespace lbui {
class Menu : public Object {
 private:
  static const Keyframe CARET;
  // Index of the currently selected element
  uint8_t selected = 0;
  // Elements in this menu
  vector<Control *> controls = {};
  // Position where the caret should be drawn
  Vector caretPosition;
  // Focuses on the element at index
  void focusItem(int index);
  // Un-focuses the element at index
  void blurItem(int index);

 public:
  Menu();
  auto eventHandler(const Event *e) -> int override;

  auto draw() -> int override;

  // Add item in the menu
  void addControl(Control* control);
  // Change position of the menu
  void setPosition(Vector p) override;
  // Set to true to draw bounding box for this element and its children
  void setDebug(bool debug) override;
};
}  // namespace lbui