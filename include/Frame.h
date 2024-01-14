#pragma once

#include <string>

#include "Colors.h"
#include "Vector.h"

namespace df {

class Frame {
 private:
  int width;
  int height;
  std::string frame_str;

 public:
  Frame();
  Frame(int w, int h, std::string frame_str);

  void setWidth(int w);
  int getWidth() const;

  void setHeight(int h);
  int getHeight() const;

  void setFrameString(std::string frame_str);
  std::string getFrameString() const;

  // Draw the frame centered at the given position with the given color
  int draw(Vector position, Color color, char transparencyChar) const;
};
}  // namespace df
