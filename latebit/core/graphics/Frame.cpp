#include "Frame.h"

#include <vector>

#include "Colors.h"
#include "DisplayManager.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {
Frame::Frame() = default;

Frame::Frame(uint8_t w, uint8_t h, vector<Color> content) {
  this->width = w;
  this->height = h;
  this->content = content;
}

auto Frame::getWidth() const -> uint8_t { return this->width; }

auto Frame::getHeight() const -> uint8_t { return this->height; }

auto Frame::getContent() const -> vector<Color> { return this->content; }

auto Frame::draw(Vector position) const -> int {
  if (this->width <= 0 || this->height <= 0 || this->content.empty()) {
    Log.warning("Frame::draw(): Won't draw empty frame");
    return 0;
  }

  if (this->width * this->height != this->content.size()) {
    Log.error("Frame::draw(): Frame content size doesn't match frame size");
    return -1;
  }

  return DM.drawFrame(position, this);
}

auto Frame::operator==(const Frame& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         equal(this->content.begin(), this->content.end(),
               other.content.begin());
}

}  // namespace lb
