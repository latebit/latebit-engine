#include "ResourceManager.h"

#include "LogManager.h"
#include "SpriteParser.h"

namespace df {

ResourceManager::ResourceManager() {
  this->sprite_count = 0;
  setType("ResourceManager");
  LM.writeLog("ResourceManager::ResourceManager(): Created ResourceManager");
}

int ResourceManager::startUp() {
  this->sprite_count = 0;
  LM.writeLog("ResourceManager::startUp(): Started successfully");
  return Manager::startUp();
}

int ResourceManager::loadSprite(std::string filename, std::string label) {
  if (this->sprite_count >= MAX_SPRITES) {
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

  this->p_sprite[this->sprite_count] = p_sprite;
  this->sprite_count++;

  return 0;
}

ResourceManager& ResourceManager::getInstance() {
  static ResourceManager instance;
  return instance;
}

int ResourceManager::unloadSprite(std::string label) {
  for (int i = 0; i < this->sprite_count; i++) {
    if (this->p_sprite[i] != nullptr &&
        this->p_sprite[i]->getLabel() == label) {
      delete this->p_sprite[i];

      // We need not scooting here, sprites are not ordered
      this->p_sprite[i] = this->p_sprite[this->sprite_count];
      this->p_sprite[this->sprite_count] = nullptr;

      this->sprite_count--;
      return 0;
    }
  }

  return -1;
}

Sprite* ResourceManager::getSprite(std::string label) const {
  for (int i = 0; i < this->sprite_count; i++) {
    if (this->p_sprite[i] == nullptr) continue;

    if (this->p_sprite[i]->getLabel() == label) {
      return this->p_sprite[i];
    }
  }

  return nullptr;
}
void ResourceManager::shutDown() {
  for (int i = 0; i < this->sprite_count; i++) {
    delete this->p_sprite[i];
  }

  this->sprite_count = 0;

  Manager::shutDown();
}

}  // namespace df
