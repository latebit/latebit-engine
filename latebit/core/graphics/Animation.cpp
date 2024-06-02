#include "Animation.h"

#include "latebit/core/GameManager.h"

namespace lb {
Animation::Animation() = default;

void Animation::setSprite(Sprite* s) {
  this->sprite = s;
  // In case the new sprite has a different number of frames, reset the index
  // and slowdown count to not have blank frames
  // For exmaple, when you try to render index 4, but the new sprite has only 3
  // frames, it will render a blank frame instead of the first one)
  this->index = 0;
  this->slowdownCount = 0;
}
auto Animation::getSprite() const -> Sprite* { return this->sprite; }

void Animation::setName(std::string n) { this->name = n; }
auto Animation::getName() const -> std::string { return this->name; }

void Animation::setIndex(int i) { this->index = i; }
auto Animation::getIndex() const -> int { return this->index; }

void Animation::setSlowdownCount(int c) { this->slowdownCount = c; }
auto Animation::getSlowdownCount() const -> int { return this->slowdownCount; }

auto Animation::draw(Vector position) -> int {
  if (this->sprite == nullptr) return -1;

  int index = getIndex();
  int result = this->sprite->draw(index, position);

  int slowdown = getSlowdownCount();
  if (slowdown == STOP_ANIMATION_SLOWDOWN) return 0;

  // Do not update the animation if the game is paused
  if (GM.isPaused()) return result;

  slowdown++;

  if (slowdown >= this->sprite->getSlowdown()) {
    slowdown = 0;
    // Circularly iterate through frames
    index = (index + 1) % this->sprite->getFrameCount();
    setIndex(index);
  }

  setSlowdownCount(slowdown);
  return result;
}

auto Animation::getBox() const -> Box {
  if (this->sprite == nullptr) return {Vector(), 1, 1};

  float width = this->sprite->getWidth();
  float height = this->sprite->getHeight();

  return {Vector(), width, height};
}

auto Animation::operator==(const Animation& other) const -> bool {
  return this->sprite == other.sprite && this->name == other.name &&
         this->index == other.index &&
         this->slowdownCount == other.slowdownCount;
}

}  // namespace lb
