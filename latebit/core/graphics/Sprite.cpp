#include "Sprite.h"

#include <string>

#include "latebit/core/geometry/Vector.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {
Sprite::Sprite(string label, uint8_t width, uint8_t height, uint8_t slowdown,
               uint8_t frameCount) {
  this->label = label;
  this->width = width;
  this->height = height;
  this->slowdown = slowdown;
  this->frames = vector<Frame>();
  this->frames.reserve(frameCount);
}

Sprite::Sprite(const Sprite& other) {
  this->width = other.width;
  this->height = other.height;
  this->slowdown = other.slowdown;
  this->label = other.label;
  this->frames = other.frames;
}

Sprite::~Sprite() = default;

auto Sprite::addFrame(Frame frame) -> int {
  if (this->frames.size() >= this->frames.capacity()) {
    Log.error("Sprite::addFrame(): Cannot add frame. Maximum %d reached",
              this->frames.capacity());
    return -1;
  }

  if (frame.getWidth() != this->width || frame.getHeight() != this->height) {
    Log.error(
      "Sprite::addFrame(): Frame dimensions (%d, %d) do not match sprite "
      "dimensions (%d, %d)",
      frame.getWidth(), frame.getHeight(), this->width, this->height);
    return -1;
  }

  this->frames.push_back(frame);

  return 0;
}

auto Sprite::getFrame(int frameNumber) const -> Frame {
  if (frameNumber < 0 || (size_t)frameNumber >= this->frames.size()) {
    Log.warning("Sprite::getFrame(): Invalid frame number (%d) with %d frames",
                frameNumber, this->frames.size());
    return {};
  }

  return this->frames[frameNumber];
}

auto Sprite::getFrameCount() const -> uint8_t { return this->frames.size(); }

void Sprite::setWidth(uint8_t width) { this->width = width; }
auto Sprite::getWidth() const -> uint8_t { return this->width; }

void Sprite::setHeight(uint8_t height) { this->height = height; }
auto Sprite::getHeight() const -> uint8_t { return this->height; }

void Sprite::setLabel(std::string label) { this->label = label; }
auto Sprite::getLabel() const -> std::string { return this->label; }

void Sprite::setSlowdown(uint8_t slowdown) { this->slowdown = slowdown; }
auto Sprite::getSlowdown() const -> uint8_t { return this->slowdown; }

auto Sprite::draw(int frameNumber, Vector position) const -> int {
  if (frameNumber < 0 || (size_t)frameNumber >= this->frames.size()) {
    Log.warning("Sprite::draw(): Invalid frame number (%d) with %d frames",
                frameNumber, this->frames.size());
    return -1;
  }

  return this->frames[frameNumber].draw(position);
}

auto Sprite::operator==(const Sprite& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         this->slowdown == other.slowdown && this->label == other.label &&
         equal(this->frames.begin(), this->frames.end(), other.frames.begin());
}

}  // namespace lb
