#include "latebit/core/GameManager.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Animation.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/graphics/Sprite.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/ui/Text.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
 public:
  Sprite *sprite;
  TestObject(Vector position, Vector velocity, Vector acceleration,
             Color::Color color, float bounciness = 1.0)
    : Object("TestObject") {
    setPosition(position);
    setVelocity(velocity);
    setAcceleration(acceleration);
    setSprite("test");

    auto animation = Animation();
    Keyframe colors(10 * 10, color);
    sprite = new Sprite("test", 10, 10, 1, {colors});
    animation.setSprite(sprite);
    setAnimation(animation);
    setBox({10, 10});
    setBounciness(bounciness);
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
  scene->createObject<lbui::Text>("one", "Keep bouncing",
                                  lbui::TextOptions{.color = Color::WHITE});
  scene->createObject<TestObject>(Vector(50, 10), Vector(), Vector(0.01, 0),
                                  Color::RED);
  scene->createObject<TestObject>(Vector(100, 10), Vector(), Vector(-0.01, 0),
                                  Color::GREEN);

  scene
    ->createObject<lbui::Text>("two", "Bounce away",
                               lbui::TextOptions{.color = Color::WHITE})
    ->setPosition({0, 30});
  scene->createObject<TestObject>(Vector(50, 40), Vector(0.5, 0), Vector(),
                                  Color::YELLOW);
  scene->createObject<TestObject>(Vector(100, 40), Vector(-0.5, 0), Vector(),
                                  Color::PINK);

  scene
    ->createObject<lbui::Text>("three", "Bidimensional hit",
                               lbui::TextOptions{.color = Color::WHITE})
    ->setPosition({0, 60});

  scene->createObject<TestObject>(Vector(110, 70), Vector(0.5, 0), Vector(),
                                  Color::BLUE);
  scene->createObject<TestObject>(Vector(140, 90), Vector(0, -0.5), Vector(),
                                  Color::DARK_PURPLE);

  scene
    ->createObject<lbui::Text>("four", "With 0 bounciness",
                               lbui::TextOptions{.color = Color::WHITE})
    ->setPosition({0, 90});

  scene->createObject<TestObject>(Vector(50, 100), Vector(), Vector(0.01, 0),
                                  Color::BROWN);
  scene->createObject<TestObject>(Vector(100, 100), Vector(), Vector(-0.01, 0),
                                  Color::PEACH, 0);

  scene
    ->createObject<lbui::Text>("five", "Parallel movement",
                               lbui::TextOptions{.color = Color::WHITE})->setPosition({0, 120});

  scene->createObject<TestObject>(Vector(50, 130), Vector(-0.05, 0), Vector(),
                                  Color::RED);
  scene->createObject<TestObject>(Vector(50, 140), Vector(0.05, 0), Vector(), Color::GREEN);

  scene->activate();
  GM.run();
  return 0;
}