#include "Sprite.h"

#include <string>

#include "Colors.h"
#include "LogManager.h"
#include "Vector.h"

namespace df {
Sprite::Sprite(int max_frames) {
  m_width = 0;
  m_height = 0;
  m_max_frame_count = max_frames;
  m_frame_count = 0;
  m_slowdown = NO_SLOWDOWN;
  m_color = COLOR_DEFAULT;
  m_frames = new Frame[m_max_frame_count];
  m_label = "";
  m_transparency_char = ' ';
}

Sprite::~Sprite() { delete[] m_frames; }

int Sprite::addFrame(Frame frame) {
  if (m_frame_count >= m_max_frame_count) {
    LM.writeLog("Sprite::addFrame(): Cannot add frame, maximum (%d) reached.",
                m_max_frame_count);
    return -1;
  }

  m_frames[m_frame_count] = frame;
  m_frame_count++;

  return 0;
}

Frame Sprite::getFrame(int frame_number) const {
  if (frame_number < 0 || frame_number >= m_frame_count) {
    LM.writeLog("Sprite::getFrame(): Invalid frame number (%d) with %d frames.",
                frame_number, m_frame_count);
    return Frame();
  }

  return m_frames[frame_number];
}

int Sprite::getFrameCount() const { return m_frame_count; }

void Sprite::setWidth(int width) { m_width = width; }
int Sprite::getWidth() const { return m_width; }

void Sprite::setHeight(int height) { m_height = height; }
int Sprite::getHeight() const { return m_height; }

void Sprite::setColor(Color color) { m_color = color; }
Color Sprite::getColor() const { return m_color; }

void Sprite::setLabel(std::string label) { m_label = label; }
std::string Sprite::getLabel() const { return m_label; }

void Sprite::setSlowdown(int slowdown) { m_slowdown = slowdown; }
int Sprite::getSlowdown() const { return m_slowdown; }

void Sprite::setTransparencyChar(char c) { m_transparency_char = c; }
char Sprite::getTransparencyChar() const { return m_transparency_char; }

int Sprite::draw(int frame_number, Vector position) const {
  if (frame_number < 0 || frame_number >= m_frame_count) {
    LM.writeLog("Sprite::draw(): Invalid frame number (%d) with %d frames.",
                frame_number, m_frame_count);
    return -1;
  }

  return m_frames[frame_number].draw(position, m_color, m_transparency_char);
}

}  // namespace df
