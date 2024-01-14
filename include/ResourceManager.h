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
  static ResourceManager &getInstance();

  int startUp() override;
  void shutDown() override;

  int loadSprite(std::string filename, std::string label);
  int unloadSprite(std::string label);

  Sprite *getSprite(std::string label) const;
};
}  // namespace df

