#pragma once

#include <functional>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Scene.h"
#include "latebit/ui/Control.h"
#include "latebit/ui/Text.h"

namespace lbui {
class Button : public Control {
 private:
  Text* text;
  function<void()> callback;

 public:
  Button(Scene* scene, const string label, function<void()> onSubmit) {
    this->text = scene->createObject<Text>(label, label);
    this->setBox(this->text->getBox());
    this->callback = onSubmit;
  }
  auto getLabel() -> string override { return this->text->getContent(); }
  void setLabel(const string l) override {
    this->text->setContent(l);
    this->setBox(text->getBox());
  }

  void setPosition(Vector p) override {
    Control::setPosition(p);
    this->text->setPosition(p);
  }

  void onSubmit() override { this->callback(); }
};
}  // namespace lbui