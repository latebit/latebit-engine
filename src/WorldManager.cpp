#include "WorldManager.h"

#include "EventCollision.h"
#include "EventOut.h"
#include "EventView.h"
#include "LogManager.h"
#include "ObjectListIterator.h"
#include "utils.h"

#define WM df::WorldManager::getInstance()

namespace df {

WorldManager::WorldManager() {
  setType("WorldManager");
  LM.writeLog("WorldManager::WorldManager(): Created WorldManager");
}

auto WorldManager::getInstance() -> WorldManager& {
  static WorldManager instance;
  return instance;
}

auto WorldManager::startUp() -> int {
  this->deletions = ObjectList();
  this->updates = ObjectList();
  LM.writeLog("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  auto all = getAllObjects();
  auto iterator = ObjectListIterator(&all);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    delete iterator.currentObject();
  }

  this->updates.clear();

  Manager::shutDown();
  LM.writeLog("WorldManager::shutDown(): Shut down successfully");
}

auto WorldManager::isValid(string eventType) const -> bool {
  return eventType == OUT_EVENT || eventType == COLLISION_EVENT ||
         eventType == VIEW_EVENT;
}

auto WorldManager::insertObject(Object* o) -> int {
  return this->sceneGraph.insertObject(o);
}

auto WorldManager::removeObject(Object* o) -> int {
  return this->sceneGraph.removeObject(o);
}

auto WorldManager::getAllObjects() const -> ObjectList {
  return this->sceneGraph.getAllObjects();
}

auto WorldManager::objectsOfType(std::string type) const -> ObjectList {
  ObjectList result;
  auto solid = this->sceneGraph.getAllObjects();
  auto iterator = ObjectListIterator(&solid);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject()->getType() == type) {
      result.insert(iterator.currentObject());
    }
  }

  return result;
}

auto WorldManager::getCollisions(Object* o, Vector where) const -> ObjectList {
  ObjectList collisions;
  auto solid = this->sceneGraph.getSolidObjects();
  auto iterator = ObjectListIterator(&solid);
  auto box = o->getWorldBox(where);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    auto current = iterator.currentObject();
    if (current == o) continue;

    auto currentBox = current->getWorldBox();

    if (intersects(box, currentBox)) {
      collisions.insert(current);
    }
  }

  return collisions;
}

auto WorldManager::moveObject(Object* o, Vector where) -> int {
  // Spectral objects can just move
  if (!o->isSolid()) {
    moveAndCheckBounds(o, where);
    return 0;
  }

  ObjectList collisions = getCollisions(o, where);

  // In absence of collisions, just move
  if (collisions.isEmpty()) {
    moveAndCheckBounds(o, where);
    return 0;
  }

  bool shouldMove = true;
  auto iterator = ObjectListIterator(&collisions);

  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    auto current = iterator.currentObject();
    auto event = EventCollision(o, current, where);
    o->eventHandler(&event);
    current->eventHandler(&event);

    // If hitting a hard object, don't move
    if (o->getSolidness() == HARD && current->getSolidness() == HARD) {
      shouldMove = false;
      break;
    }
  }

  if (shouldMove) {
    moveAndCheckBounds(o, where);
    return 0;
  } else {
    return -1;
  }

  return 0;
}

void WorldManager::moveAndCheckBounds(Object* o, Vector where) {
  auto initial = o->getWorldBox();
  o->setPosition(where);
  auto final = o->getWorldBox();
  auto boundary = WM.getBoundary();

  if (intersects(initial, boundary) && !intersects(final, boundary)) {
    auto event = EventOut();
    o->eventHandler(&event);
  }

  if (this->viewFollowing == o) {
    auto viewDeadZone = this->getViewDeadZone();

    // Move the view if the object is outside of the dead zone
    if (!contains(viewDeadZone, final)) {
      setViewPosition(where);
    }
  }
}

void WorldManager::update() {
  if (!isStarted()) return;

  auto deletions = ObjectListIterator(&this->deletions);
  for (deletions.first(); !deletions.isDone(); deletions.next()) {
    removeObject(deletions.currentObject());
    delete deletions.currentObject();
  }
  this->deletions.clear();

  auto all = getAllObjects();
  auto updates = ObjectListIterator(&all);
  for (updates.first(); !updates.isDone(); updates.next()) {
    auto object = updates.currentObject();
    auto oldPosition = object->getPosition();
    auto newPosition = object->predictPosition();

    if (oldPosition != newPosition) {
      moveObject(object, newPosition);
    }
  }
}

auto WorldManager::markForDelete(Object* o) -> int {
  // Prevents marking the same object for deletion twice
  auto iterator = ObjectListIterator(&this->deletions);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject() == o) return 0;
  }

  return this->deletions.insert(o);
}

void WorldManager::draw() {
  // auto iterator = ObjectListIterator(&this->updates);

  for (int i = 0; i <= MAX_ALTITUDE; i++) {
    auto visible = this->getSceneGraph().getVisibleObjects(i);
    auto iterator = ObjectListIterator(&visible);

    for (iterator.first(); !iterator.isDone(); iterator.next()) {
      auto object = iterator.currentObject();
      if (object != nullptr && intersects(object->getWorldBox(), this->view)) {
        object->draw();
      };
    }
  }
}

void WorldManager::setBoundary(Box b) { this->boundary = b; }
auto WorldManager::getBoundary() const -> Box { return this->boundary; }

void WorldManager::setView(Box v) { this->view = v; }
auto WorldManager::getView() const -> Box { return this->view; }

auto WorldManager::setViewPosition(Vector where) -> void {
  auto viewHeight = this->view.getHeight();
  auto viewWidth = this->view.getWidth();
  auto worldHeight = this->boundary.getHeight();
  auto worldWidth = this->boundary.getWidth();
  auto worldOrigin = this->boundary.getCorner();
  auto worldOriginX = worldOrigin.getX();
  auto worldOriginY = worldOrigin.getY();

  // easier to understand as the following translations
  // viewCenter = corner + Vector(width / 2, height / 2)
  // translate = where - viewCenter (note, `where` is the center of the view)
  // newCorner = viewCorner + translate
  auto newCorner = where - Vector(viewWidth / 2, viewHeight / 2);

  newCorner.setX(clamp(newCorner.getX(), worldOriginX,
                       worldOriginX + worldWidth - viewWidth));
  newCorner.setY(clamp(newCorner.getY(), worldOriginY,
                       worldOriginY + worldHeight - viewHeight));

  this->view = Box(newCorner, viewWidth, viewHeight);
}

auto WorldManager::setViewFollowing(Object* o) -> int {
  if (o == nullptr) {
    this->viewFollowing = nullptr;
    return 0;
  }

  auto updates = getAllObjects();
  auto iterator = ObjectListIterator(&updates);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    if (iterator.currentObject() == o) {
      this->viewFollowing = o;
      return 0;
    }
  }

  LM.writeLog(
    "WorldManager::setViewFollowing(): Object to be followed not found.");
  return -1;
}

void WorldManager::setViewDeadZone(Box d) {
  if (!contains(this->view, d)) {
    LM.writeLog("WorldManager::setViewDeadZone(): Dead zone larger than view.");
    return;
  }

  this->viewDeadZone = d;
}

auto WorldManager::getViewDeadZone() const -> Box { return this->viewDeadZone; }

auto WorldManager::getSceneGraph() -> SceneGraph& { return this->sceneGraph; }
}  // namespace df
