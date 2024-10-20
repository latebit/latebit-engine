#include "latebit/core/GameManager.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Animation.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/graphics/Sprite.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
 public:
  Sprite *sprite;
  TestObject(Vector position, Vector velocity, Vector acceleration, Color::Color color, int size = 5) : Object("TestObject") {
    setPosition(position);
    setVelocity(velocity);
    setAcceleration(acceleration);
    setSprite("test");

    auto animation = Animation();
    Keyframe colors(size * size, color);
    sprite = new Sprite("test", size, size, 1, {colors});
    animation.setSprite(sprite);
    setAnimation(animation);
    setBox({(float)size, (float)size});
  }

  ~TestObject() {
    delete sprite;
    Object::~Object();
  };
};

auto main() -> int {
  Log.setDestination(LogDestination::STDOUT);

  GM.startUp();
  
  auto scene = WM.createScene<Scene>("main");
  scene->createObject<TestObject>(Vector(50, 10), Vector(), Vector(0.01, 0), Color::RED, 10);
  scene->createObject<TestObject>(Vector(100, 10), Vector(), Vector(-0.01, 0), Color::GREEN, 10);

  scene->createObject<TestObject>(Vector(50, 40), Vector(1, 0), Vector(), Color::YELLOW, 10);
  scene->createObject<TestObject>(Vector(100, 40), Vector(-1, 0), Vector(), Color::PINK, 10);
  scene->activate();
  GM.run();
  return 0;
}