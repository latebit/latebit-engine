#include "Dialog.h"

#include "latebit/core/events/EventInput.h"
#include "latebit/core/events/EventStep.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/ui/Rectangle.h"
#include "latebit/utils/Math.h"

using namespace lb;

namespace lbui {

// clang-format off
Icon ICON = {
    {
      Color::BLACK,           Color::BLACK,           Color::BLACK, Color::BLACK,           Color::BLACK,           Color::UNDEFINED_COLOR,
      Color::UNDEFINED_COLOR, Color::BLACK,           Color::BLACK, Color::BLACK,           Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR,
      Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::BLACK, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR, Color::UNDEFINED_COLOR},
    6, 3};
// clang-format on

Dialog::Dialog(Scene* scene, vector<string> pages, std::function<void()> onEnd,
               RectangleOptions options) {
  this->rectangle = scene->createObject<Rectangle>(options);
  this->setBox(this->rectangle->getBox());
  this->pages = pages;
  this->onEnd = onEnd;
  this->setActive(false);
  subscribe(INPUT_EVENT);
  subscribe(STEP_EVENT);
}

void Dialog::reset() {
  this->currentPage = 0;
  this->canChangePage = false;
  this->renderedBuffer = "";
  this->caretDeltaY = 0;

  this->setActive(false);
  this->onEnd();
}

auto Dialog::eventHandler(const Event* e) -> int {
  if (e->getType() == INPUT_EVENT) {
    const auto event = static_cast<const EventInput*>(e);
    auto action = event->getAction();
    auto key = event->getKey();

    if (action == lb::InputAction::PRESSED && key == lb::InputKey::START) {
      if (canChangePage) {
        // Change page
        this->currentPage++;
        this->renderedBuffer = "";
        if (this->currentPage >= pages.size()) {
          // Close the dialog and fire callback
          this->reset();
        }
      } else {
        // Show the whole page
        this->renderedBuffer = pages[currentPage];
      }
      return 1;
    }
  }

  if (e->getType() == STEP_EVENT) {
    const auto event = static_cast<const EventStep*>(e);
    if (event->getStepCount() % 30 == 0) {
      this->caretDeltaY =
        static_cast<float>((static_cast<int>(this->caretDeltaY + 1) & 1));
      return 1;
    }

    if (event->getStepCount() % 3 == 0) {
      if (pages.size() == 0) {
        return 0;
      }
      auto page = pages[currentPage];
      auto currentPageSize = page.size();
      auto displayedLength =
        clamp(renderedBuffer.size() + 1, 0, currentPageSize);
      this->canChangePage = (uint8_t)displayedLength == currentPageSize;
      this->renderedBuffer = page.substr(0, displayedLength);

      return 1;
    }
  }

  return 0;
}

auto Dialog::draw() -> int {
  int result = 0;
  const auto rect = this->rectangle->getWorldBox();
  const auto caretPosition = rect.getCorner() +
                             Vector{rect.getWidth(), rect.getHeight()} -
                             PADDING - Vector{2, 0};
  const auto textPosition = WM.getCamera().viewToWorld(rect.getCorner() + PADDING);

  result += caret.draw(caretPosition + Vector{0, caretDeltaY});
  result += DM.drawString(textPosition, renderedBuffer, TextAlignment::LEFT,
                          Color::BLACK);
  return result;
}

auto Dialog::setActive(bool active) -> int {
  this->rectangle->setActive(active);
  Object::setActive(active);
  this->setVisible(active);
  return 0;
}

auto Dialog::setVisible(bool visible) -> int {
  // Note: this is order dependent!
  // Making the rectangle visible after the object will render the text
  // underneath the rectangle
  this->rectangle->setVisible(visible);
  Object::setVisible(visible);
  return 0;
}

auto Dialog::setPosition(Vector position) -> void {
  this->rectangle->setPosition(position);
  // We need to make sure the object is moved as well, else we skip drawing it
  // when it's not part of the current view
  Object::setPosition(position);
}

}  // namespace lbui