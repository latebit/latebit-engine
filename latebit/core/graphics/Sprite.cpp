#include "Sprite.h"

#include <string>

#include "latebit/core/geometry/Vector.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {
auto Sprite::getFrame(int index) const -> Keyframe {
  if (index < 0 || (size_t)index >= this->frames.size()) {
    Log.warning("Sprite::getFrame(): Invalid frame number (%d) with %d frames",
                index, this->frames.size());
    return {};
  }

  return this->frames[index];
}

auto Sprite::getFrameCount() const -> uint8_t { return this->frames.size(); }
auto Sprite::getWidth() const -> uint8_t { return this->width; }
auto Sprite::getHeight() const -> uint8_t { return this->height; }
auto Sprite::getDuration() const -> uint8_t { return this->duration; }

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
