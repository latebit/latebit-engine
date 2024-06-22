#include "Keyframe.h"

#include <vector>

#include "Colors.h"
#include "DisplayManager.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {
Keyframe::Keyframe() = default;

Keyframe::Keyframe(uint8_t w, uint8_t h, vector<Color::Color> content) {
  this->width = w;
  this->height = h;
  this->content = content;
}

auto Keyframe::getWidth() const -> uint8_t { return this->width; }

auto Keyframe::getHeight() const -> uint8_t { return this->height; }

auto Keyframe::getContent() const -> vector<Color::Color> {
  return this->content;
}

auto Keyframe::draw(Vector position) const -> int {
  if (this->width <= 0 || this->height <= 0 || this->content.empty()) {
    Log.warning("Keyframe::draw(): Won't draw empty frame");
    return 0;
  }

  if (this->width * this->height != this->content.size()) {
    Log.error(
      "Keyframe::draw(): Keyframe content size doesn't match frame size");
    return -1;
  }

  return DM.drawFrame(position, this);
}

auto Keyframe::operator==(const Keyframe& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         equal(this->content.begin(), this->content.end(),
               other.content.begin());
}

}  // namespace lb
