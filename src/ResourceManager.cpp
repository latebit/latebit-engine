#include "ResourceManager.h"

#include "LogManager.h"
#include "SpriteParser.h"

namespace df {

ResourceManager::ResourceManager() {
  m_sprite_count = 0;
  setType("ResourceManager");
  LM.writeLog("ResourceManager::ResourceManager(): Created ResourceManager");
}

int ResourceManager::startUp() {
  m_sprite_count = 0;
  LM.writeLog("ResourceManager::startUp(): Started successfully");
  return Manager::startUp();
}

int ResourceManager::loadSprite(std::string filename, std::string label) {
  if (m_sprite_count >= MAX_SPRITES) {
    LM.writeLog(
        "ResourceManager::loadSprite(): Cannot load sprite, maximum (%d) "
        "reached.",
        MAX_SPRITES);
    return -1;
  }

  if (getSprite(label) != nullptr) {
    LM.writeLog(
        "ResourceManager::loadSprite(): Cannot load sprite, label '%s' "
        "already in use.",
        label.c_str());
    return -1;
  }

  Sprite* p_sprite = SpriteParser::parseSprite(filename, label);

  if (p_sprite == nullptr) {
    LM.writeLog("ResourceManager::loadSprite(): could not parse sprite '%s'.",
                label.c_str());
    return -1;
  }

  m_p_sprite[m_sprite_count] = p_sprite;
  m_sprite_count++;

  return 0;
}

ResourceManager& ResourceManager::getInstance() {
  static ResourceManager instance;
  return instance;
}

int ResourceManager::unloadSprite(std::string label) {
  for (int i = 0; i < m_sprite_count; i++) {
    if (m_p_sprite[i] != nullptr && m_p_sprite[i]->getLabel() == label) {
      delete m_p_sprite[i];

      // We need not scooting here, sprites are not ordered
      m_p_sprite[i] = m_p_sprite[m_sprite_count];
      m_p_sprite[m_sprite_count] = nullptr;

      m_sprite_count--;
      return 0;
    }
  }

  return -1;
}

Sprite* ResourceManager::getSprite(std::string label) const {
  for (int i = 0; i < m_sprite_count; i++) {
    if (m_p_sprite[i] == nullptr) continue;

    if (m_p_sprite[i]->getLabel() == label) {
      return m_p_sprite[i];
    }
  }

  return nullptr;
}
void ResourceManager::shutDown() {
  for (int i = 0; i < m_sprite_count; i++) {
    if (m_p_sprite[i] != nullptr) {
      delete m_p_sprite[i];
    }
  }

  m_sprite_count = 0;

  Manager::shutDown();
}

}  // namespace df
