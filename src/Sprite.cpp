#include "Sprite.h"

#include <string>

#include "Colors.h"
#include "LogManager.h"
#include "Vector.h"

namespace df {
Sprite::Sprite(int max_frames) {
  this->width = 0;
  this->height = 0;
  this->max_frame_count = max_frames;
  this->frame_count = 0;
  this->slowdown = NO_SLOWDOWN;
  this->color = COLOR_DEFAULT;
  this->frames = new Frame[this->max_frame_count];
  this->label = "";
  this->transparency_char = ' ';
}

Sprite::~Sprite() { delete[] this->frames; }

auto Sprite::addFrame(Frame frame) -> int {
  if (this->frame_count >= this->max_frame_count) {
    LM.writeLog("Sprite::addFrame(): Cannot add frame, maximum (%d) reached.",
                this->max_frame_count);
    return -1;
  }

  this->frames[this->frame_count] = frame;
  this->frame_count++;

  return 0;
}

auto Sprite::getFrame(int frame_number) const -> Frame {
  if (frame_number < 0 || frame_number >= this->frame_count) {
    LM.writeLog("Sprite::getFrame(): Invalid frame number (%d) with %d frames.",
                frame_number, this->frame_count);
    return {};
  }

  return this->frames[frame_number];
}

auto Sprite::getFrameCount() const -> int { return this->frame_count; }

void Sprite::setWidth(int width) { this->width = width; }
auto Sprite::getWidth() const -> int { return this->width; }

void Sprite::setHeight(int height) { this->height = height; }
auto Sprite::getHeight() const -> int { return this->height; }

void Sprite::setColor(Color color) { this->color = color; }
auto Sprite::getColor() const -> Color { return this->color; }

void Sprite::setLabel(std::string label) { this->label = label; }
auto Sprite::getLabel() const -> std::string { return this->label; }

void Sprite::setSlowdown(int slowdown) { this->slowdown = slowdown; }
auto Sprite::getSlowdown() const -> int { return this->slowdown; }

void Sprite::setTransparencyChar(char c) { this->transparency_char = c; }
auto Sprite::getTransparencyChar() const -> char { return this->transparency_char; }

auto Sprite::draw(int frame_number, Vector position) const -> int {
  if (frame_number < 0 || frame_number >= this->frame_count) {
    LM.writeLog("Sprite::draw(): Invalid frame number (%d) with %d frames.",
                frame_number, this->frame_count);
    return -1;
  }

  return this->frames[frame_number].draw(position, this->color,
                                         this->transparency_char);
}

}  // namespace df
