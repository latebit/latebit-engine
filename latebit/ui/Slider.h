#include "Control.h"
#include "latebit/core/world/Scene.h"
#include "latebit/ui/Icon.h"
#include "latebit/ui/Text.h"

namespace lbui {
class Slider : public Control {
  // Icons representing the bars
  static const array<Icon, 2> BARS;
  // The text component representing the label 
  Text* text;
  // Function fired every time a value changes
  function<void(int, int)> callback;
  // The value controlled by this control
  int value = 0;
 
 public:
  Slider(Scene* scene, const string label, int initialValue,
         function<void(int, int)> onChanged);
  
  // Returns the label of the control
  auto getLabel() -> string;
  
  // Sets the label of the control
  void setLabel(const string l);

  // Sets position of the control and child elements
  void setPosition(Vector p) override;
  
  // Called when the value has decreased
  void onChange(int v) override;

  // Draw the slider
  auto draw() -> int override;
  
  // Return the value of this control
  auto getValue() -> int;
};
}  // namespace lbui