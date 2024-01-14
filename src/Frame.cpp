#include "Frame.h"

#include "Colors.h"
#include "DisplayManager.h"
#include "LogManager.h"
#include "Vector.h"

namespace df {
Frame::Frame() {
  this->width = 0;
  this->height = 0;
  this->frame_str = "";
}

Frame::Frame(int w, int h, std::string frame_str) {
  this->width = w;
  this->height = h;
  this->frame_str = frame_str;
}

void Frame::setWidth(int w) { this->width = w; }
int Frame::getWidth() const { return this->width; }

void Frame::setHeight(int w) { this->height = w; }
int Frame::getHeight() const { return this->height; }

void Frame::setFrameString(std::string frame_str) {
  this->frame_str = frame_str;
}
std::string Frame::getFrameString() const { return this->frame_str; }

int Frame::draw(Vector position, Color color, char transparencyChar) const {
  if (this->width <= 0 || this->height <= 0 || this->frame_str.empty()) {
    LM.writeLog("Frame::draw(): Cannot draw empty frame");
    return -1;
  }

  float startX = position.getX();
  float startY = position.getY();

  for (int i = 0; i < this->height; i++) {
    for (int j = 0; j < this->width; j++) {
      Vector temp(startX + j, startY + i);
      char ch = this->frame_str[i * this->width + j];

      if (ch == transparencyChar) continue;

      DM.drawCh(temp, ch, color);
    }
  }

  return 0;
}

}  // namespace df
