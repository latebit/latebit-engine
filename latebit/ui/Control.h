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
  // Fired when the control changes value
  virtual void onChange() {};

  virtual auto getLabel() -> string { return getType(); };
  virtual void setLabel(const string s) { setType(s); };
};
}  // namespace lbui