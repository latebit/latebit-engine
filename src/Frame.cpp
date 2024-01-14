#include "Frame.h"

#include "Colors.h"
#include "DisplayManager.h"
#include "LogManager.h"
#include "Vector.h"

namespace df {
Frame::Frame() {
  m_width = 0;
  m_height = 0;
  m_frame_str = "";
}

Frame::Frame(int w, int h, std::string frame_str) {
  m_width = w;
  m_height = h;
  m_frame_str = frame_str;
}

void Frame::setWidth(int w) { m_width = w; }
int Frame::getWidth() const { return m_width; }

void Frame::setHeight(int w) { m_height = w; }
int Frame::getHeight() const { return m_height; }

void Frame::setFrameString(std::string frame_str) { m_frame_str = frame_str; }
std::string Frame::getFrameString() const { return m_frame_str; }

int Frame::draw(Vector position, Color color, char transparencyChar) const {
  if (m_width <= 0 || m_height <= 0 || m_frame_str.empty()) {
    LM.writeLog("Frame::draw(): Cannot draw empty frame");
    return -1;
  }

  float startX = position.getX();
  float startY = position.getY();

  for (int i = 0; i < m_height; i++) {
    for (int j = 0; j < m_width; j++) {
      Vector temp(startX + j, startY + i);
      char ch = m_frame_str[i * m_width + j];

      if (ch == transparencyChar) continue;

      DM.drawCh(temp, ch, color);
    }
  }

  return 0;
}

}  // namespace df
