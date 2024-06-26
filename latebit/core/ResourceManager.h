#pragma once

#include "latebit/core/audio/Music.h"
#include "latebit/core/audio/Sound.h"
#include "latebit/core/graphics/Animation.h"
#include "latebit/core/graphics/Sprite.h"
#include "latebit/core/utils/Manager.h"

using namespace std;

namespace lb {
const int MAX_SPRITES = 500;
const int MAX_SOUNDS = 50;
const int MAX_MUSICS = 50;

class ResourceManager : public Manager {
 private:
  ResourceManager();
  // Loaded sound buffers
  vector<unique_ptr<Sound>> sound = {};
  // Loaded music
  vector<unique_ptr<Music>> music = {};
  // Loaded sprites
  vector<unique_ptr<Sprite>> sprite = {};

 public:
  ResourceManager(ResourceManager const &) = delete;
  void operator=(ResourceManager const &) = delete;
  static auto getInstance() -> ResourceManager &;

  auto startUp() -> int override;
  void shutDown() override;

  // Load a text sprite by its filename and associate it with a label
  auto loadTextSprite(string filename, string label) -> int;

  // Load a 16-colors PNG sprite by its filename and associate it with a label
  // The PNG can have multiple frames, in which case we will assume they all are
  // on the same line and divide the image width by the number of frames
  auto loadImageSprite(string filename, string label, int frames = 1,
                       int slowDown = STOP_ANIMATION_SLOWDOWN) -> int;

  // Frees the sprite label and associated resources
  auto unloadSprite(string label) -> int;
  // Return pointer to sprite associated with label
  [[nodiscard]] auto getSprite(string label) const -> const Sprite *;

  // Load a sound by its filename and associate it with a label
  auto loadSound(string filename, string label) -> int;
  // Frees the sound label and associated resources
  auto unloadSound(string label) -> int;
  // Return pointer to sound associated with label
  [[nodiscard]] auto getSound(string label) const -> const Sound *;

  // Load a music by its filename and associate it with a label
  auto loadMusic(string filename, string label) -> int;
  // Frees the music label and associated resources
  auto unloadMusic(string label) -> int;
  // Return pointer to music associated with label
  [[nodiscard]] auto getMusic(string label) const -> const Music *;
};
}  // namespace lb

#define RM lb::ResourceManager::getInstance()