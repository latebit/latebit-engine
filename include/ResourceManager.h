#pragma once

#include "Manager.h"
#include "Music.h"
#include "Sound.h"
#include "Sprite.h"

using namespace std;

#define RM lb::ResourceManager::getInstance()

namespace lb {
const int MAX_SPRITES = 500;
const int MAX_SOUNDS = 50;
const int MAX_MUSICS = 50;

class ResourceManager : public Manager {
 private:
  ResourceManager();
  // Loaded sound buffers
  array<Sound *, MAX_SOUNDS> sound = {};
  // Number of loaded sound buffers
  int soundCount = 0;

  // Loaded music
  array<Music *, MAX_MUSICS> music = {};
  // Number of loaded musics
  int musicCount = 0;

  // Loaded sprites
  array<Sprite *, MAX_SPRITES> sprite = {};
  // Number of loaded sprites
  int spriteCount = 0;

 public:
  ResourceManager(ResourceManager const &) = delete;
  void operator=(ResourceManager const &) = delete;
  static auto getInstance() -> ResourceManager &;

  auto startUp() -> int override;
  void shutDown() override;

  // Load a sprite by its filename and associate it with a label
  auto loadSprite(string filename, string label) -> int;
  // Frees the sprite label and associated resources
  auto unloadSprite(string label) -> int;
  // Return pointer to sprite associated with label
  [[nodiscard]] auto getSprite(string label) const -> Sprite *;

  // Load a sound by its filename and associate it with a label
  auto loadSound(string filename, string label) -> int;
  // Frees the sound label and associated resources
  auto unloadSound(string label) -> int;
  // Return pointer to sound associated with label
  [[nodiscard]] auto getSound(string label) const -> Sound *;

  // Load a music by its filename and associate it with a label
  auto loadMusic(string filename, string label) -> int;
  // Frees the music label and associated resources
  auto unloadMusic(string label) -> int;
  // Return pointer to music associated with label
  [[nodiscard]] auto getMusic(string label) const -> Music *;
};
}  // namespace lb
