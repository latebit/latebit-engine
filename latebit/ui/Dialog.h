
#include <functional>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/Scene.h"
#include "latebit/ui/Icon.h"
#include "latebit/ui/Rectangle.h"

using namespace std;

namespace lbui {

// clang-format off
extern Icon ICON;
// clang-format on

class Dialog : public Object {
 private:
  const Vector PADDING = {10, 10};

  // Rectangle that will be used to draw the dialog
  Rectangle* rectangle;
  // Icon that will be used to draw the caret
  Icon caret = ICON;

  // Current page being displayed
  uint8_t currentPage = 0;
  // Set to true when the user can move to the next page
  bool canChangePage = false;
  // Portion of the page that will be rendered in the dialog
  string renderedBuffer = "";
  // Position of the caret. Used to animate the caret
  float caretDeltaY = 0;

  // Pages of the dialog
  vector<string> pages = {};

  // Callback that will be called when the dialog ends
  function<void()> onEnd;

  // Reset the dialog to its initial state
  void reset();

 public:
  Dialog(Scene* scene, vector<string> pages, function<void()> onEnd,
         RectangleOptions options = RectangleOptions{
           .height = 40,
           .width = WINDOW_WIDTH - 2,
           .fill = Color::WHITE,
           .stroke = {Color::BLACK, Color::WHITE, Color::BLACK, Color::BLACK}});

  auto eventHandler(const Event* e) -> int override;

  // Draw the dialog and its subcomponents
  auto draw() -> int override;

  auto setActive(bool active) -> int override;

  auto setPosition(Vector position) -> void override;
};

}  // namespace lbui