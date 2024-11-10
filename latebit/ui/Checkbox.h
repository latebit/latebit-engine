#pragma once

#include <functional>
#include <string>

#include "latebit/core/world/Scene.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Icon.h"
#include "latebit/ui/Text.h"

using namespace std;

namespace lbui {

class Checkbox : public Control {
 private:
  static const array<Icon, 2> CHECKS;
  Text* text;
  function<void(bool)> callback;
  bool value;

 public:
  Checkbox(Scene* scene, const string label, bool initialValue,
           function<void(bool)> onSubmit);

  // Returns the label of the control
  auto getLabel() -> string;

  // Set the label of the control
  void setLabel(const string l);

  // Set the position of the control and its children
  void setPosition(Vector p) override;

  // Fired when the checkbox value changes
  void onSubmit() override;

  // Draw the Checkbox
  auto draw() -> int override;

  // Returns the value of this control
  auto getValue() -> bool;
};

}  // namespace lbui