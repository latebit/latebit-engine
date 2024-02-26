#include "ResourceManager.h"

#include "Logger.h"
#include "SpriteParser.h"

namespace lb {

ResourceManager::ResourceManager() {
  setType("ResourceManager");
  Log.debug("ResourceManager::ResourceManager(): Created ResourceManager");
}

auto ResourceManager::startUp() -> int {
  this->spriteCount = 0;
  this->soundCount = 0;
  this->musicCount = 0;
  Log.info("ResourceManager::startUp(): Started successfully");
  return Manager::startUp();
}

void ResourceManager::shutDown() {
  for (int i = 0; i < this->spriteCount; i++) {
    delete this->sprite[i];
  }

  for (int i = 0; i < this->soundCount; i++) {
    delete this->sound[i];
  }

  for (int i = 0; i < this->musicCount; i++) {
    delete this->music[i];
  }

  this->spriteCount = 0;
  this->soundCount = 0;
  this->musicCount = 0;

  Manager::shutDown();
  Log.info("ResourceManager::shutDown(): Shut down successfully");
}

auto ResourceManager::loadSprite(string filename, string label) -> int {
  if (this->spriteCount >= MAX_SPRITES) {
    Log.error("ResourceManager::loadSprite(): Cannot load sprite. Maximum",
              MAX_SPRITES, "sprites already reached");
    return -1;
  }

  if (getSprite(label) != nullptr) {
    Log.error("ResourceManager::loadSprite(): Cannot load sprite, label", label,
              "already in use");
    return -1;
  }

  auto* sprite = new Sprite(SpriteParser::parseSprite(filename, label));

  this->sprite[this->spriteCount] = sprite;
  this->spriteCount++;

  return 0;
}

auto ResourceManager::getInstance() -> ResourceManager& {
  static ResourceManager instance;
  return instance;
}

auto ResourceManager::unloadSprite(string label) -> int {
  for (int i = 0; i < this->spriteCount; i++) {
    if (this->sprite[i] != nullptr && this->sprite[i]->getLabel() == label) {
      delete this->sprite[i];

      // We need not scooting here, sprites are not ordered
      this->sprite[i] = this->sprite[this->spriteCount];
      this->sprite[this->spriteCount] = nullptr;

      this->spriteCount--;
      return 0;
    }
  }

  return -1;
}

auto ResourceManager::getSprite(string label) const -> Sprite* {
  for (int i = 0; i < this->spriteCount; i++) {
    if (this->sprite[i] == nullptr) continue;

    if (this->sprite[i]->getLabel() == label) {
      return this->sprite[i];
    }
  }

  Log.debug("ResourceManager::getSprite(): Unable to find sprite with label",
            label);
  return nullptr;
}

auto ResourceManager::loadSound(string filename, string label) -> int {
  if (this->soundCount >= MAX_SOUNDS) {
    Log.error("ResourceManager::loadSound(): Cannot load sound. Maximum",
              MAX_SOUNDS, "sounds already reached");
    return -1;
  }

  if (getSound(label) != nullptr) {
    Log.error("ResourceManager::loadSound(): Cannot load sound, label", label,
              "already in use");
    return -1;
  }

  this->sound[this->soundCount] = new Sound();

  if (this->sound[this->soundCount]->loadSound(filename) != 0) {
    Log.error("ResourceManager::loadSound(): Could not load sound", label);
    return -1;
  };

  this->sound[this->soundCount]->setLabel(label);
  this->soundCount++;

  return 0;
}

auto ResourceManager::unloadSound(string label) -> int {
  for (int i = 0; i < this->soundCount; i++) {
    auto sound = this->sound[i];
    if (sound != nullptr && sound->getLabel() == label) {
      delete sound;

      // We need not scooting here, sounds are not ordered
      this->sound[i] = this->sound[this->soundCount];
      this->sound[this->soundCount] = nullptr;
      this->soundCount--;

      return 0;
    }
  }

  Log.debug("ResourceManager::unloadSound(): Unable to find sound with label",
            label);
  return -1;
}

auto ResourceManager::getSound(string label) const -> Sound* {
  for (int i = 0; i < this->soundCount; i++) {
    if (this->sound[i] != nullptr && this->sound[i]->getLabel() == label) {
      return this->sound[i];
    }
  }

  Log.debug("ResourceManager::getSound(): Unable to find sound with label",
            label);
  return nullptr;
}

auto ResourceManager::loadMusic(string filename, string label) -> int {
  if (this->musicCount >= MAX_MUSICS) {
    Log.error("ResourceManager::loadMusic(): Cannot load sound, maximum (",
              MAX_MUSICS, ") reached.");
    return -1;
  }

  if (getMusic(label) != nullptr) {
    Log.error("ResourceManager::loadMusic(): Cannot load music, label", label,
              "already in use");
    return -1;
  }

  this->music[this->musicCount] = new Music();

  if (this->music[this->musicCount]->loadMusic(filename) != 0) {
    Log.error("ResourceManager::loadMusic(): Could not load file", filename);
    return -1;
  };

  this->music[this->musicCount]->setLabel(label);
  this->musicCount++;

  return 0;
}

auto ResourceManager::unloadMusic(string label) -> int {
  for (int i = 0; i < this->musicCount; i++) {
    auto music = this->music[i];
    if (music != nullptr && music->getLabel() == label) {
      delete music;

      // We need not scooting here, musics are not ordered
      this->music[i] = this->music[this->musicCount];
      this->music[this->musicCount] = nullptr;
      this->musicCount--;

      return 0;
    }
  }

  Log.debug("ResourceManager::unloadMusic(): Unable to find music with label",
            label);
  return -1;
}

auto ResourceManager::getMusic(string label) const -> Music* {
  for (int i = 0; i < this->musicCount; i++) {
    if (this->music[i] != nullptr && this->music[i]->getLabel() == label) {
      return this->music[i];
    }
  }

  Log.debug("ResourceManager::getMusic(): unable to find music with label",
            label);
  return nullptr;
}

}  // namespace lb
