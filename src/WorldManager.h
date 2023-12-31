#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__

#include "Manager.h"
#include "ObjectList.h"

#define WM df::WorldManager::getInstance()

namespace df {

class WorldManager : public Manager {
 private:
  WorldManager();
  WorldManager(WorldManager const &);
  void operator=(WorldManager const &);

  ObjectList m_updates;
  ObjectList m_deletions;

 public:
  static WorldManager &getInstance();

  int startUp();
  void shutDown();
  int insertObject(Object *p_o);
  int removeObject(Object *p_o);
  ObjectList getAllObjects() const;
  ObjectList objectsOfType(std::string type) const;

  void update();
  int markForDelete(Object *p_o);
};
}  // namespace df

#endif