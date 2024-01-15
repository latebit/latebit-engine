#include "ResourceManager.h"

#include "LogManager.h"
#include "SpriteParser.h"

namespace df {

ResourceManager::ResourceManager() {
  this->sprite_count = 0;
  setType("ResourceManager");
  LM.writeLog("ResourceManager::ResourceManager(): Created ResourceManager");
}

auto ResourceManager::startUp() -> int {
  this->sprite_count = 0;
  LM.writeLog("ResourceManager::startUp(): Started successfully");
  return Manager::startUp();
}

auto ResourceManager::loadSprite(std::string filename, std::string label)
  -> int {
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

  Sprite* sprite = SpriteParser::parseSprite(filename, label);

  if (sprite == nullptr) {
    LM.writeLog("ResourceManager::loadSprite(): could not parse sprite '%s'.",
                label.c_str());
    return -1;
  }

  this->sprite[this->sprite_count] = sprite;
  this->sprite_count++;

  return 0;
}

auto ResourceManager::getInstance() -> ResourceManager& {
  static ResourceManager instance;
  return instance;
}

auto ResourceManager::unloadSprite(std::string label) -> int {
  for (int i = 0; i < this->sprite_count; i++) {
    if (this->sprite[i] != nullptr && this->sprite[i]->getLabel() == label) {
      delete this->sprite[i];

      // We need not scooting here, sprites are not ordered
      this->sprite[i] = this->sprite[this->sprite_count];
      this->sprite[this->sprite_count] = nullptr;

      this->sprite_count--;
      return 0;
    }
  }

  return -1;
}

auto ResourceManager::getSprite(std::string label) const -> Sprite* {
  for (int i = 0; i < this->sprite_count; i++) {
    if (this->sprite[i] == nullptr) continue;

    if (this->sprite[i]->getLabel() == label) {
      return this->sprite[i];
    }
  }

  return nullptr;
}
void ResourceManager::shutDown() {
  for (int i = 0; i < this->sprite_count; i++) {
    delete this->sprite[i];
  }

  this->sprite_count = 0;

  Manager::shutDown();
}

}  // namespace df
