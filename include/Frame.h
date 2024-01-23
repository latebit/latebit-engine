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
  Frame(int w, int h, string content);

  void setWidth(int w);
  [[nodiscard]] auto getWidth() const -> int;

  void setHeight(int h);
  [[nodiscard]] auto getHeight() const -> int;

  void setFrameString(string content);
  [[nodiscard]] auto getFrameString() const -> string;

  // Draw the frame centered at the given position with the given color
  [[nodiscard]] auto draw(Vector position, Color color,
                          char transparencyChar) const -> int;
};
}  // namespace df
