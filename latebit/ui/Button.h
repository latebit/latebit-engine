#pragma once

#include <functional>
#include <string>

#include "latebit/core/world/Scene.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Text.h"

namespace lbui {

class Button : public Control {
 private:
  Text* text;
  std::function<void()> callback;

 public:
  Button(Scene* scene, const std::string label, std::function<void()> onSubmit);
  auto getLabel() -> std::string override;
  void setLabel(const std::string l) override;
  void setPosition(Vector p) override;
  void onSubmit() override;
};

}  // namespace lbui