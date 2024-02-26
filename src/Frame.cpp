#include "Frame.h"

#include <vector>

#include "Colors.h"
#include "DisplayManager.h"
#include "Logger.h"
#include "Vector.h"

using namespace std;

namespace lb {
Frame::Frame() = default;

Frame::Frame(int w, int h, vector<Color> content) {
  this->width = w;
  this->height = h;
  this->content = content;
}

auto Frame::getWidth() const -> int { return this->width; }

auto Frame::getHeight() const -> int { return this->height; }

auto Frame::getContent() const -> vector<Color> { return this->content; }

auto Frame::draw(Vector position) const -> int {
  if (this->width <= 0 || this->height <= 0 || this->content.empty()) {
    Log.warning("Frame::draw(): Won't draw empty frame");
    return 0;
  }

  return DM.drawFrame(position, this);
}

auto Frame::operator==(const Frame& other) const -> bool {
  return this->width == other.width && this->height == other.height &&
         equal(this->content.begin(), this->content.end(),
               other.content.begin());
}

}  // namespace lb
