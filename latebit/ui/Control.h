#pragma once

#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
class Control : public Object {
 public:
  // Fired when the control is focused
  virtual void onFocus() {};
  // Fired when the control stops being focused
  virtual void onBlur() {};
  // Fired when the control is focused and START is pressed
  virtual void onSubmit() {};
};
}  // namespace lbui