#include "Sprite.h"

#include <string>

#include "latebit/core/geometry/Vector.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {
Sprite::Sprite(string label, uint8_t width, uint8_t height, uint8_t duration,
               uint8_t frameCount) {
  this->label = label;
  this->width = width;
  this->height = height;
  this->duration = duration;
  this->frames = vector<Keyframe>();
  this->frames.reserve(frameCount);
}

Sprite::Sprite(const Sprite& other) {
  this->width = other.width;
  this->height = other.height;
  this->duration = other.duration;
  this->label = other.label;
  this->frames = other.frames;
}

Sprite::~Sprite() = default;

auto Sprite::addFrame(Keyframe frame) -> int {
  if (this->frames.size() >= this->frames.capacity()) {
    Log.error("Sprite::addFrame(): Cannot add frame. Maximum %d reached",
              this->frames.capacity());
    return -1;
  }

  if (frame.getWidth() != this->width || frame.getHeight() != this->height) {
    Log.error(
      "Sprite::addFrame(): Keyframe dimensions (%d, %d) do not match sprite "
      "dimensions (%d, %d)",
      frame.getWidth(), frame.getHeight(), this->width, this->height);
    return -1;
  }

  this->frames.push_back(frame);

  return 0;
}

auto Sprite::getFrame(int index) const -> Keyframe {
  if (index < 0 || (size_t)index >= this->frames.size()) {
    Log.warning("Sprite::getFrame(): Invalid frame number (%d) with %d frames",
                index, this->frames.size());
    return {};
  }

  return this->frames[index];
}

auto Sprite::setFrame(int index, Keyframe frame) -> int {
  if (index < 0 || (size_t)index >= this->frames.size()) {
    Log.warning("Sprite::setFrame(): Invalid frame number (%d) with %d frames",
                index, this->frames.size());
    return -1;
  }

  if (frame.getWidth() != this->width || frame.getHeight() != this->height) {
    Log.error(
      "Sprite::setFrame(): Keyframe dimensions (%d, %d) do not match sprite "
      "dimensions (%d, %d)",
      frame.getWidth(), frame.getHeight(), this->width, this->height);
    return -1;
  }

  this->frames[index] = frame;

  return 0;
}

auto Sprite::getFrameCount() const -> uint8_t { return this->frames.size(); }
auto Sprite::getWidth() const -> uint8_t { return this->width; }
auto Sprite::getHeight() const -> uint8_t { return this->height; }
auto Sprite::getDuration() const -> uint8_t { return this->duration; }

void Sprite::setLabel(std::string label) { this->label = label; }
auto Sprite::getLabel() const -> std::string { return this->label; }

auto Sprite::draw(int index, Vector position) const -> int {
  if (index < 0 || (size_t)index >= this->frames.size()) {
    Log.warning("Sprite::draw(): Invalid frame number (%d) with %d frames",
                index, this->frames.size());
    return -1;
  }

  return this->frames[index].draw(position);
}

auto Sprite::operator==(const Sprite& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         this->duration == other.duration && this->label == other.label &&
         equal(this->frames.begin(), this->frames.end(), other.frames.begin());
}

}  // namespace lb
