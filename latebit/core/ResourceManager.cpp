#include "ResourceManager.h"

#include "latebit/core/graphics/SpriteParser.h"
#include "latebit/utils/Logger.h"

namespace lb {

ResourceManager::ResourceManager() : Manager("ResourceManager") {
  Log.debug("ResourceManager::ResourceManager(): Created ResourceManager");
}

auto ResourceManager::startUp() -> int {
  this->sprite.reserve(MAX_SPRITES);
  this->sound.reserve(MAX_SOUNDS);
  this->music.reserve(MAX_MUSICS);

  Log.info("ResourceManager::startUp(): Started successfully");
  return Manager::startUp();
}

void ResourceManager::shutDown() {
  this->sprite.clear();
  this->sound.clear();
  this->music.clear();

  Manager::shutDown();
  Log.info("ResourceManager::shutDown(): Shut down successfully");
}

auto ResourceManager::loadTextSprite(string filename, string label) -> int {
  if (this->sprite.size() >= MAX_SPRITES) {
    Log.error(
      "ResourceManager::loadSprite(): Cannot load sprite. Maximum %d sprites "
      "reached",
      MAX_SPRITES);
    return -1;
  }

  if (getSprite(label) != nullptr) {
    Log.error(
      "ResourceManager::loadSprite(): Cannot load sprite, label %s already in "
      "use",
      label.c_str());
    return -1;
  }

  Sprite sprite = SpriteParser::fromTextFile(filename, label);
  if (sprite == Sprite()) {
    Log.error("ResourceManager::loadSprite(): Could not load sprite %s",
              label.c_str());
    return -1;
  }

  this->sprite.push_back(make_unique<Sprite>(sprite));

  return 0;
}

auto ResourceManager::loadImageSprite(string filename, string label, int frames,
                                      int duration) -> int {
  if (frames < 1) {
    Log.error(
      "ResourceManager::loadImageSprite(): Cannot load sprite. Number of "
      "frames must be greater than 0");
    return -1;
  }

  if (this->sprite.size() >= MAX_SPRITES) {
    Log.error(
      "ResourceManager::loadSprite(): Cannot load sprite. Maximum %d sprites "
      "reached",
      MAX_SPRITES);
    return -1;
  }

  if (getSprite(label) != nullptr) {
    Log.error(
      "ResourceManager::loadSprite(): Cannot load sprite, label %s already in "
      "use",
      label.c_str());
    return -1;
  }

  Sprite sprite = SpriteParser::fromPNGFile(filename, label, frames, duration);
  if (sprite == Sprite()) {
    Log.error("ResourceManager::loadSprite(): Could not load sprite %s",
              label.c_str());
    return -1;
  }

  this->sprite.push_back(make_unique<Sprite>(sprite));

  return 0;
}

auto ResourceManager::getInstance() -> ResourceManager& {
  static ResourceManager instance;
  return instance;
}

auto ResourceManager::unloadSprite(string label) -> int {
  for (size_t i = 0; i < this->sprite.size(); i++) {
    if (this->sprite[i] != nullptr && this->sprite[i]->getLabel() == label) {
      // We need not scooting here, sprites are not ordered
      this->sprite[i] = std::move(this->sprite.back());
      this->sprite.back() = nullptr;
      return 0;
    }
  }

  return -1;
}

auto ResourceManager::getSprite(string label) const -> const Sprite* {
  for (size_t i = 0; i < this->sprite.size(); i++) {
    if (this->sprite[i] == nullptr) continue;

    if (this->sprite[i]->getLabel() == label) {
      return this->sprite[i].get();
    }
  }

  Log.debug("ResourceManager::getSprite(): Unable to find sprite with label %s",
            label.c_str());
  return nullptr;
}

auto ResourceManager::loadSound(string filename, string label) -> int {
  if (this->sound.size() >= MAX_SOUNDS) {
    Log.error(
      "ResourceManager::loadSound(): Cannot load sound. Maximum %d sound "
      "reached",
      MAX_SOUNDS);
    return -1;
  }

  if (getSound(label) != nullptr) {
    Log.error(
      "ResourceManager::loadSound(): Cannot load sound, label %s already in "
      "use",
      label.c_str());
    return -1;
  }

  auto tune = TuneParser::fromFile(filename, &SOUND_PARSER_OPTIONS);
  if (tune == nullptr) {
    Log.error("ResourceManager::loadSound(): Could not load sound %s",
              label.c_str());
    return -1;
  };

  this->sound.push_back(make_unique<Sound>(label, std::move(tune)));
  return 0;
}

auto ResourceManager::unloadSound(string label) -> int {
  for (size_t i = 0; i < this->sound.size(); i++) {
    if (this->sound[i] != nullptr && this->sound[i]->getLabel() == label) {
      // We need not scooting here, sounds are not ordered
      this->sound[i] = std::move(this->sound.back());
      this->sound.back() = nullptr;
      return 0;
    }
  }

  Log.debug(
    "ResourceManager::unloadSound(): Unable to find sound with label %s",
    label.c_str());
  return -1;
}

auto ResourceManager::getSound(string label) const -> const Sound* {
  for (size_t i = 0; i < this->sound.size(); i++) {
    if (this->sound[i] != nullptr && this->sound[i]->getLabel() == label) {
      return this->sound[i].get();
    }
  }

  Log.debug("ResourceManager::getSound(): Unable to find sound with label %s",
            label.c_str());
  return nullptr;
}

auto ResourceManager::loadMusic(string filename, string label) -> int {
  if (this->music.size() >= MAX_MUSICS) {
    Log.error(
      "ResourceManager::loadMusic(): Cannot load sound. Maximum %d musics "
      "reached",
      MAX_MUSICS);
    return -1;
  }

  if (getMusic(label) != nullptr) {
    Log.error(
      "ResourceManager::loadMusic(): Cannot load music, label %s already in "
      "use",
      label.c_str());
    return -1;
  }

  auto tune = TuneParser::fromFile(filename, &MUSIC_PARSER_OPTIONS);
  if (tune == nullptr) {
    Log.error("ResourceManager::loadMusic(): Could not load sound %s",
              label.c_str());
    return -1;
  };

  this->music.push_back(make_unique<Music>(label, std::move(tune)));
  return 0;
}

auto ResourceManager::unloadMusic(string label) -> int {
  for (size_t i = 0; i < this->music.size(); i++) {
    if (this->music[i] != nullptr && this->music[i]->getLabel() == label) {
      // We need not scooting here, musics are not ordered
      this->music[i] = std::move(this->music.back());
      this->music.back() = nullptr;
      return 0;
    }
  }

  Log.debug(
    "ResourceManager::unloadMusic(): Unable to find music with label %s",
    label.c_str());
  return -1;
}

auto ResourceManager::getMusic(string label) const -> const Music* {
  for (size_t i = 0; i < this->music.size(); i++) {
    if (this->music[i] != nullptr && this->music[i]->getLabel() == label) {
      return this->music[i].get();
    }
  }

  Log.debug("ResourceManager::getMusic(): Unable to find music with label %s",
            label.c_str());
  return nullptr;
}

}  // namespace lb
