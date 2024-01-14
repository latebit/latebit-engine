#pragma once

#include "Manager.h"
#include "Sprite.h"

#define RM df::ResourceManager::getInstance()

namespace df {
const int MAX_SPRITES = 500;

class ResourceManager : public Manager {
 private:
  ResourceManager();
  ResourceManager(ResourceManager const &);
  void operator=(ResourceManager const &);
  Sprite *sprite[MAX_SPRITES];
  int sprite_count;

 public:
  static auto getInstance() -> ResourceManager &;

  auto startUp() -> int override;
  void shutDown() override;

  auto loadSprite(std::string filename, std::string label) -> int;
  auto unloadSprite(std::string label) -> int;

  auto getSprite(std::string label) const -> Sprite *;
};
}  // namespace df
