#include "Animation.h"

namespace df {
Animation::Animation() {
  this->p_sprite = nullptr;
  this->name = "";
  this->index = 0;
  this->slowdownCount = 0;
}

void Animation::setSprite(Sprite* p_s) { this->p_sprite = p_s; }
Sprite* Animation::getSprite() const { return this->p_sprite; }

void Animation::setName(std::string n) { this->name = n; }
std::string Animation::getName() const { return this->name; }

void Animation::setIndex(int i) { this->index = i; }
int Animation::getIndex() const { return this->index; }

void Animation::setSlowdownCount(int c) { this->slowdownCount = c; }
int Animation::getSlowdownCount() const { return this->slowdownCount; }

int Animation::draw(Vector position) {
  if (this->p_sprite == nullptr) return -1;

  int index = getIndex();
  this->p_sprite->draw(index, position);

  int slowdown = getSlowdownCount();
  if (slowdown == STOP_ANIMATION_SLOWDOWN) return 0;

  slowdown++;

  if (slowdown >= this->p_sprite->getSlowdown()) {
    slowdown = 0;
    // Circularly iterate through frames
    index = (index + 1) % this->p_sprite->getFrameCount();
    setIndex(index);
  }

  setSlowdownCount(slowdown);
  return 0;
}

Box Animation::getBox() const {
  if (this->p_sprite == nullptr) return Box(Vector(), 1, 1);

  auto width = this->p_sprite->getWidth();
  auto height = this->p_sprite->getHeight();

  return Box(Vector(), width, height);
}

bool Animation::operator==(const Animation& other) const {
  return this->p_sprite == other.p_sprite && this->name == other.name &&
         this->index == other.index &&
         this->slowdownCount == other.slowdownCount;
}

}  // namespace df
