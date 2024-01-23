#include "Frame.h"

#include "Colors.h"
#include "DisplayManager.h"
#include "LogManager.h"
#include "Vector.h"

using namespace std;

namespace df {
Frame::Frame() = default;

Frame::Frame(int w, int h, string content) {
  this->width = w;
  this->height = h;
  this->content = content;
}

void Frame::setWidth(int w) { this->width = w; }
auto Frame::getWidth() const -> int { return this->width; }

void Frame::setHeight(int w) { this->height = w; }
auto Frame::getHeight() const -> int { return this->height; }

void Frame::setFrameString(string content) { this->content = content; }
auto Frame::getFrameString() const -> string { return this->content; }

auto Frame::draw(Vector position, Color color, char transparencyChar) const
  -> int {
  if (this->width <= 0 || this->height <= 0 || this->content.empty()) {
    LM.writeLog("Frame::draw(): Cannot draw empty frame");
    return -1;
  }

  float startX = position.getX();
  float startY = position.getY();

  for (int i = 0; i < this->height; i++) {
    for (int j = 0; j < this->width; j++) {
      Vector temp(startX + j, startY + i);
      char ch = this->content[i * this->width + j];

      if (ch == transparencyChar) continue;

      DM.drawCh(temp, ch, color);
    }
  }

  return 0;
}

}  // namespace df
