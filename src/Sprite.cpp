#include "Sprite.h"

#include <string>

#include "Colors.h"
#include "LogManager.h"
#include "Vector.h"

namespace df {
Sprite::Sprite(int maxFrames) {
  this->maxFrameCount = maxFrames;
  this->frames = std::vector<Frame>(maxFrames);
}

Sprite::Sprite(const Sprite& other) {
  this->width = other.width;
  this->height = other.height;
  this->maxFrameCount = other.maxFrameCount;
  this->frameCount = other.frameCount;
  this->slowdown = other.slowdown;
  this->color = other.color;
  this->label = other.label;
  this->transparencyChar = other.transparencyChar;
  this->frames = other.frames;
}

Sprite::~Sprite() = default;

auto Sprite::addFrame(Frame frame) -> int {
  if (this->frameCount >= this->maxFrameCount) {
    LM.writeLog("Sprite::addFrame(): Cannot add frame, maximum (%d) reached.",
                this->maxFrameCount);
    return -1;
  }

  this->frames[this->frameCount] = frame;
  this->frameCount++;

  return 0;
}

auto Sprite::getFrame(int frameNumber) const -> Frame {
  if (frameNumber < 0 || frameNumber >= this->frameCount) {
    LM.writeLog("Sprite::getFrame(): Invalid frame number (%d) with %d frames.",
                frameNumber, this->frameCount);
    return {};
  }

  return this->frames[frameNumber];
}

auto Sprite::getFrameCount() const -> int { return this->frameCount; }

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

void Sprite::setTransparencyCharachter(char c) { this->transparencyChar = c; }
auto Sprite::getTransparencyCharchter() const -> char {
  return this->transparencyChar;
}

auto Sprite::draw(int frameNumber, Vector position) const -> int {
  if (frameNumber < 0 || frameNumber >= this->frameCount) {
    LM.writeLog("Sprite::draw(): Invalid frame number (%d) with %d frames.",
                frameNumber, this->frameCount);
    return -1;
  }

  return this->frames[frameNumber].draw(position, this->color,
                                        this->transparencyChar);
}

auto Sprite::operator==(const Sprite& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         this->maxFrameCount == other.maxFrameCount &&
         this->frameCount == other.frameCount &&
         this->slowdown == other.slowdown && this->color == other.color &&
         this->label == other.label &&
         this->transparencyChar == other.transparencyChar;
}

}  // namespace df
