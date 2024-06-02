#pragma once

#include <vector>

#include "Colors.h"
#include "latebit/core/geometry/Vector.h"

using namespace std;

namespace lb {

class Frame {
 private:
  uint8_t width = 0;
  uint8_t height = 0;
  vector<Color> content = {};

 public:
  // Frames are used to render a given frame of a sprite animation.
  // Creates a zero frame with no content
  Frame();
  // Frames are used to render a given frame of a sprite animation.
  // Creates a new frame with the given width, height, and content.
  Frame(uint8_t w, uint8_t h, vector<Color> content);

  // Get the width of the frame
  [[nodiscard]] auto getWidth() const -> uint8_t;

  // Get the height of the frame
  [[nodiscard]] auto getHeight() const -> uint8_t;

  // Get the content of the frame
  [[nodiscard]] auto getContent() const -> vector<Color>;

  // Draws the frame at the given position with the given color.
  // The position is the top left corner of the frame.
  [[nodiscard]] auto draw(Vector position) const -> int;

  // Compares two frames for equality
  auto operator==(const Frame& other) const -> bool;
};
}  // namespace lb
