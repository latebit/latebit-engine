#include "Animation.h"

#include <cstdint>

#include "latebit/core/GameManager.h"

namespace lb {
Animation::Animation() = default;

void Animation::setSprite(const Sprite* s) {
  this->sprite = s;
  // In case the new sprite has a different number of frames, reset the index
  // and duration count to not have blank frames
  // For example, when you try to render index 4, but the new sprite has only 3
  // frames, it will render a blank frame instead of the first one)
  this->index = 0;
  this->slowdownCount = 0;
}
auto Animation::getSprite() const -> const Sprite* { return this->sprite; }

void Animation::setName(std::string n) { this->name = n; }
auto Animation::getName() const -> std::string { return this->name; }

void Animation::setIndex(int i) { this->index = i; }
auto Animation::getIndex() const -> int { return this->index; }

void Animation::setSlowdownCount(int c) { this->slowdownCount = c; }
auto Animation::getSlowdownCount() const -> int { return this->slowdownCount; }

auto Animation::draw(Vector position, uint8_t scale) -> int {
  if (this->sprite == nullptr) return -1;
  if (this->sprite->getFrameCount() == 0) return -1;

  int index = getIndex();
  int result = this->sprite->drawKeyframe(index, position, scale);

  int duration = getSlowdownCount();
  if (duration == STOP_ANIMATION_SLOWDOWN) return 0;

  // Do not update the animation if the game is paused
  if (GM.isPaused()) return result;

  duration++;

  if (duration >= this->sprite->getDuration()) {
    duration = 0;
    // Circularly iterate through frames
    index = (index + 1) % this->sprite->getFrameCount();
    setIndex(index);
  }

  setSlowdownCount(duration);
  return result;
}

auto Animation::operator==(const Animation& other) const -> bool {
  return this->sprite == other.sprite && this->name == other.name &&
         this->index == other.index &&
         this->slowdownCount == other.slowdownCount;
}

}  // namespace lb
