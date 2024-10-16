#include "Sprite.h"

#include <cstdint>
#include <string>

#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/DisplayManager.h"
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

auto Sprite::drawKeyframe(int index, Vector position, uint8_t scale) const
  -> int {
  auto frame = this->getFrame(index);
  return DM.drawKeyframe(position, &frame, this->width, this->height,
                                scale);
}

auto Sprite::operator==(const Sprite& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         this->duration == other.duration && this->label == other.label &&
         equal(this->frames.begin(), this->frames.end(), other.frames.begin());
}

}  // namespace lb
