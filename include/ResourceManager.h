#pragma once

#include "Manager.h"
#include "Music.h"
#include "Sound.h"
#include "Sprite.h"

#define RM df::ResourceManager::getInstance()

namespace df {
const int MAX_SPRITES = 500;
const int MAX_SOUNDS = 50;
const int MAX_MUSICS = 50;

class ResourceManager : public Manager {
 private:
  ResourceManager();
  ResourceManager(ResourceManager const &) = delete;
  void operator=(ResourceManager const &) = delete;

  // Loaded sound buffers
  std::array<Sound *, MAX_SOUNDS> sound = {};
  // Number of loaded sound buffers
  int soundCount = 0;

  // Loaded music
  std::array<Music *, MAX_MUSICS> music = {};
  // Number of loaded musics
  int musicCount = 0;

  // Loaded sprites
  std::array<Sprite *, MAX_SPRITES> sprite = {};
  // Number of loaded sprites
  int spriteCount = 0;

 public:
  static auto getInstance() -> ResourceManager &;

  auto startUp() -> int override;
  void shutDown() override;

  // Load a sprite by its filename and associate it with a label
  auto loadSprite(std::string filename, std::string label) -> int;
  // Frees the sprite label and associated resources
  auto unloadSprite(std::string label) -> int;
  // Return pointer to sprite associated with label
  [[nodiscard]] auto getSprite(std::string label) const -> Sprite *;

  // Load a sound by its filename and associate it with a label
  auto loadSound(std::string filename, std::string label) -> int;
  // Frees the sound label and associated resources
  auto unloadSound(std::string label) -> int;
  // Return pointer to sound associated with label
  [[nodiscard]] auto getSound(std::string label) const -> Sound *;

  // Load a music by its filename and associate it with a label
  auto loadMusic(std::string filename, std::string label) -> int;
  // Frees the music label and associated resources
  auto unloadMusic(std::string label) -> int;
  // Return pointer to music associated with label
  [[nodiscard]] auto getMusic(std::string label) const -> Music *;
};
}  // namespace df
