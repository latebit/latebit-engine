#pragma once

#include <functional>
#include <string>

#include "latebit/core/world/Scene.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Menu.h"
#include "latebit/ui/Text.h"

using namespace std;

namespace lbui {

class Checkbox : public Control {
 private:
  static const array<Icon, 2> CHECKS; 
  Text* text;
  function<void()> callback;
  bool value;

 public:
  Checkbox(Scene* scene, const string label, bool initialValue, function<void()> onSubmit);
  auto getLabel() -> string;
  void setLabel(const string l);
  void setPosition(Vector p) override;
  void onSubmit() override;
  auto draw() -> int override;

  auto getValue() -> bool;
};

}  // namespace lbui