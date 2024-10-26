#pragma once

#include <functional>
#include <string>

#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/world/Scene.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Text.h"

using namespace std;

namespace lbui {

class Checkbox : public Control {
 private:
  Text* text;
  function<void()> callback;
  bool value;
  const static array<Keyframe, 2> CHECKS; 

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