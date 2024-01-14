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

int Sprite::addFrame(Frame frame) {
  if (this->frame_count >= this->max_frame_count) {
    LM.writeLog("Sprite::addFrame(): Cannot add frame, maximum (%d) reached.",
                this->max_frame_count);
    return -1;
  }

  this->frames[this->frame_count] = frame;
  this->frame_count++;

  return 0;
}

Frame Sprite::getFrame(int frame_number) const {
  if (frame_number < 0 || frame_number >= this->frame_count) {
    LM.writeLog("Sprite::getFrame(): Invalid frame number (%d) with %d frames.",
                frame_number, this->frame_count);
    return Frame();
  }

  return this->frames[frame_number];
}

int Sprite::getFrameCount() const { return this->frame_count; }

void Sprite::setWidth(int width) { this->width = width; }
int Sprite::getWidth() const { return this->width; }

void Sprite::setHeight(int height) { this->height = height; }
int Sprite::getHeight() const { return this->height; }

void Sprite::setColor(Color color) { this->color = color; }
Color Sprite::getColor() const { return this->color; }

void Sprite::setLabel(std::string label) { this->label = label; }
std::string Sprite::getLabel() const { return this->label; }

void Sprite::setSlowdown(int slowdown) { this->slowdown = slowdown; }
int Sprite::getSlowdown() const { return this->slowdown; }

void Sprite::setTransparencyChar(char c) { this->transparency_char = c; }
char Sprite::getTransparencyChar() const { return this->transparency_char; }

int Sprite::draw(int frame_number, Vector position) const {
  if (frame_number < 0 || frame_number >= this->frame_count) {
    LM.writeLog("Sprite::draw(): Invalid frame number (%d) with %d frames.",
                frame_number, this->frame_count);
    return -1;
  }

  return this->frames[frame_number].draw(position, this->color, this->transparency_char);
}

}  // namespace df
