#pragma once

#include "Text.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/Scene.h"

using namespace lb;

namespace lbui {
class Menu : public Object {
 private:
  // Space between elements in the menu
  const int GAP = 2;
  // Character used to highlight menu element
  const string BULLET = ">";

  // Index of the currently selected element
  uint8_t selected = 0;
  // Elements in this menu
  vector<Text *> items = {};
  // Callbacks to be fired when an element is selected. They are in the same
  // order as the items.
  vector<function<void()>> callbacks = {};
  // Scene this object belongs to
  Scene *scene = nullptr;

  // Focuses on the element at index
  void focusItem(int index);
  // Un-focuses the element at index
  void blurItem(int index);

 public:
  Menu(Scene *scene);
  auto eventHandler(const Event *e) -> int override;

  // Add item in the menu
  void addItem(string label, function<void()> onSelected);
  // Change position of the menu
  void setPosition(Vector p) override;
  // Set to true to draw bounding box for this element and its children
  void setDebug(bool debug) override;
};
}  // namespace lbui