#pragma once

#include <string>

#include "Colors.h"
#include "Vector.h"

using namespace std;

namespace df {

class Frame {
 private:
  int width = 0;
  int height = 0;
  string content = "";

 public:
  Frame();
  // Frames are used to render a given frame of a sprite animation.
  // Creates a new frame with the given width, height, and content
  Frame(int w, int h, string content);

  // Set the width of the frame
  void setWidth(int w);
  // Get the width of the frame
  [[nodiscard]] auto getWidth() const -> int;

  // Set the height of the frame
  void setHeight(int h);
  // Get the height of the frame
  [[nodiscard]] auto getHeight() const -> int;

  // Set the content of the frame
  void setFrameString(string content);
  // Get the content of the frame
  [[nodiscard]] auto getFrameString() const -> string;

  // Draws the frame at the given position with the given color.
  // The position is the top left corner of the frame.
  [[nodiscard]] auto draw(Vector position, Color color,
                          char transparencyChar) const -> int;
};
}  // namespace df
