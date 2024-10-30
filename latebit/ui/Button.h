#pragma once

#include <functional>
#include <string>

#include "latebit/core/world/Scene.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Text.h"

using namespace std;

namespace lbui {

class Button : public Control {
 private:
  Text* text;
  function<void()> callback;

 public:
  Button(Scene* scene, const string label, function<void()> onSubmit);
  auto getLabel() -> string;
  void setLabel(const string l);
  void setPosition(Vector p) override;
  void onSubmit() override;
};

}  // namespace lbui