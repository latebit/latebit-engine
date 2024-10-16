#include "Object.h"

#include <cstdint>
#include <string>

#include "latebit/core/GameManager.h"
#include "latebit/core/ResourceManager.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"

using namespace std;

namespace lb {

Object::Object() : sceneGraph(&WM.getSceneGraph()) {
  static int id = 0;
  this->id = id++;
}

Object::Object(const string& type) : Object() { this->type = type; }

auto Object::getId() const -> int { return this->id; }

void Object::setType(string t) { this->type = t; }
auto Object::getType() const -> string { return this->type; }

void Object::setPosition(Vector p) { this->position = p; }
auto Object::getPosition() const -> Vector { return this->position; }

void Object::setScale(uint8_t scale) { this->scale = scale; }
auto Object::getScale() const -> uint8_t { return this->scale; }

void Object::setAltitude(int a) {
  if (a <= MAX_ALTITUDE && a >= 0) {
    this->sceneGraph->setAltitude(this, a);
    this->altitude = a;
  }
}
auto Object::getAltitude() const -> int { return this->altitude; }

void Object::setVelocity(Vector v) { this->velocity = v; }
auto Object::getVelocity() const -> Vector { return this->velocity; }

void Object::setAcceleration(Vector a) { this->acceleration = a; }
auto Object::getAcceleration() const -> Vector { return this->acceleration; }

auto Object::isSolid() const -> bool {
  return this->solidness != Solidness::SPECTRAL;
}

void Object::setSolidness(Solidness::Solidness s) {
  this->sceneGraph->setSolidness(this, s);
  this->solidness = s;
}
auto Object::getSolidness() const -> Solidness::Solidness {
  return this->solidness;
}

void Object::setAnimation(Animation a) { this->animation = a; }
auto Object::getAnimation() const -> Animation { return this->animation; }

auto Object::setSprite(string label) -> int {
  auto s = RM.getSprite(label);
  if (s == nullptr) return -1;

  this->animation.setSprite(s);
  setBox({Vector(), (float)s->getWidth(), (float)s->getHeight()});

  return 0;
}

void Object::setBox(Box box) { this->boundingBox = box; }
auto Object::getBox() const -> Box { return this->boundingBox; }

auto Object::getWorldBox() const -> Box { return getWorldBox(this->position); }
auto Object::getWorldBox(Vector center) const -> Box {
  auto boundingBox = this->boundingBox;
  auto corner = boundingBox.getCorner() + center;
  return {corner, boundingBox.getWidth(), boundingBox.getHeight()};
}

auto Object::eventHandler([[maybe_unused]] const Event* e) -> int { return 0; }

auto Object::draw() -> int {
  Vector p = getPosition();

  int result = this->animation.draw(p, this->scale);
  if (this->debug) {
    result += drawBoundingBox();
  }

  return result;
}

void Object::setDebug(bool debug) { this->debug = debug; }
auto Object::getDebug() const -> bool { return this->debug; }

auto Object::drawBoundingBox() const -> int {
  Box box = getWorldBox();
  Vector corner = box.getCorner();
  float width = box.getWidth();
  float height = box.getHeight();

  return DM.drawRectangle(corner, width, height, Color::RED);
}

auto Object::subscribe(string eventType) -> int {
  if (this->eventCount >= MAX_EVENTS_PER_OBEJCT) {
    Log.error(
      "Object::subscribe(): Cannot subscribe. %s has reached maximum "
      "subscriptions %d",
      MAX_EVENTS_PER_OBEJCT, this->toString().c_str());
    return -1;
  }

  this->events[this->eventCount] = eventType;
  this->eventCount++;

  if (IM.subscribe(this, eventType) == 0) {
    return 0;
  }

  if (GM.subscribe(this, eventType) == 0) {
    return 0;
  }

  // WM handles custom (user defined) events, hence it's the fallback
  return WM.subscribe(this, eventType);
}

auto Object::unsubscribe(string eventType) -> int {
  for (int i = 0; i < this->eventCount; i++) {
    if (this->events[i] == eventType) {
      this->events[i] = this->events[this->eventCount - 1];
      this->eventCount--;
      break;
    }
  }

  if (IM.unsubscribe(this, eventType) == 0) {
    return 0;
  }

  if (GM.unsubscribe(this, eventType) == 0) {
    return 0;
  }

  // WM handles custom (user defined) events, hence it's the fallback
  return WM.unsubscribe(this, eventType);
}

auto Object::unsubscribeAll() -> int {
  int result = 0;
  auto count = this->eventCount;
  for (int i = 0; i < count; i++) {
    result |= unsubscribe(this->events[i]);
  }
  return result;
}

auto Object::setActive(bool active) -> int {
  this->sceneGraph->setActive(this, active);
  this->active = active;
  return 0;
}
auto Object::isActive() const -> bool { return this->active; }

auto Object::setVisible(bool visible) -> int {
  this->sceneGraph->setVisible(this, visible);
  this->visible = visible;
  return 0;
}
auto Object::isVisible() const -> bool { return this->visible; }

auto Object::toString() const -> string {
  return "Object(id=" + to_string(this->id) + ",type=" + this->type + ")";
}

}  // namespace lb
