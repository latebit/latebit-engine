#include "Animation.h"

namespace df {
Animation::Animation() {
  m_p_sprite = nullptr;
  m_name = "";
  m_index = 0;
  m_slowdown_count = 0;
}

void Animation::setSprite(Sprite* p_s) { m_p_sprite = p_s; }
Sprite* Animation::getSprite() const { return m_p_sprite; }

void Animation::setName(std::string n) { m_name = n; }
std::string Animation::getName() const { return m_name; }

void Animation::setIndex(int i) { m_index = i; }
int Animation::getIndex() const { return m_index; }

void Animation::setSlowdownCount(int c) { m_slowdown_count = c; }
int Animation::getSlowdownCount() const { return m_slowdown_count; }

int Animation::draw(Vector position) {
  if (m_p_sprite == nullptr) return -1;

  int index = getIndex();
  m_p_sprite->draw(index, position);

  int slowdown = getSlowdownCount();
  if (slowdown == STOP_ANIMATION_SLOWDOWN) return 0;

  slowdown++;

  if (slowdown >= m_p_sprite->getSlowdown()) {
    slowdown = 0;
    // Circularly iterate through frames
    index = (index + 1) % m_p_sprite->getFrameCount();
    setIndex(index);
  }

  setSlowdownCount(slowdown);
  return 0;
}

Box Animation::getBox() const {
  if (m_p_sprite == nullptr) return Box(Vector(), 1, 1);

  auto width = m_p_sprite->getWidth();
  auto height = m_p_sprite->getHeight();

  return Box(Vector(), width, height);
}

bool Animation::operator==(const Animation& other) const {
  return m_p_sprite == other.m_p_sprite && m_name == other.m_name &&
         m_index == other.m_index && m_slowdown_count == other.m_slowdown_count;
}

}  // namespace df
