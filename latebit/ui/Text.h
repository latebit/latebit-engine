#pragma once

#include <string>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Object.h"

using namespace lb;

namespace lbui {
class TextOptions {
 public:
  // Text size, according to the DisplayManager
  TextSize::TextSize size = TextSize::NORMAL;
  // Text alignment, according to the DisplayManager.
  //
  // Please note
  //   This compounds with the `setPositionOnScreen` utils; results might be
  //   suprising
  TextAlignment::TextAlignment alignment = TextAlignment::LEFT;
  // Color of the text
  Color::Color color = Color::WHITE;
  // Color of the shadow of the text. Use Color::UNDEFINED_COLOR for no shadow.
  Color::Color shadow = Color::UNDEFINED_COLOR;
};

const Vector SHADOW_POSITION = {2, 2};

class Text : public Object {
  // Text to be displayed
  string content;
  // Rendering option for the text
  TextOptions options;

 public:
  Text(const string label, const string text,
       const TextOptions options = TextOptions());

  // Draws this object on the screen
  auto draw() -> int override;

  // Sets the text to be rendered
  void setContent(const string text);

  // Gets the text to be rendered
  [[nodiscard]] auto getContent() -> string;

  void setPosition(Vector p) override;
};
}  // namespace lbui