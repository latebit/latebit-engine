#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/events/EventStep.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"
#include "latebit/utils/Math.h"

using namespace lb;

class Player : public Object {
 public:
  bool stopAtZero = false;

  Player() {
    setAcceleration({0, 0.1});
    setBodyType(lb::BodyType::DYNAMIC);
    subscribe(INPUT_EVENT);
    subscribe(STEP_EVENT);
    setBox({ 10, 10 });
    setDebug(true);
  }

  auto eventHandler(const Event *e) -> int override {
    if (e->getType() == INPUT_EVENT) {
      auto input = static_cast<const EventInput *>(e);
      if (input->getAction() == lb::InputAction::PRESSED)
        return onInputPressed(input->getKey());
      
      if (input->getAction() == lb::InputAction::RELEASED)
        return onInputReleased(input->getKey());
    }

    if (e->getType() == STEP_EVENT) {
      return onStep(static_cast<const EventStep *>(e));
    }

    return 0;
  }

  auto onInputPressed(const InputKey::InputKey key) -> int {
    switch(key) {
      case InputKey::A:
        if (getVelocity().getY() == 0)
          setVelocity({getVelocity().getX(), -3});
        return 1;
      case InputKey::LEFT:
        setVelocity({-1.2, getVelocity().getY()});
        return 1;
      case InputKey::RIGHT:
        setVelocity({1.2, getVelocity().getY()});
        return 1;
      default:
        return 0;
    }
  }

  auto onInputReleased(const InputKey::InputKey key) -> int {
    switch (key) {
      case InputKey::RIGHT:
        setAcceleration({ -0.05, getAcceleration().getY() });
        stopAtZero = true;
        return 1;
      case InputKey::LEFT:
        setAcceleration({ 0.05, getAcceleration().getY() });
        stopAtZero = true;
        return 1;
      default:
        return 0; 
    }
  }

  auto onStep(const EventStep*) -> int {
    if (stopAtZero) {
      if (equals(getVelocity().getX(), 0)) {
        setAcceleration({0, getAcceleration().getY()});
        stopAtZero = false;
      }
    }
    return 0;
  }
};

auto main() -> int {
  Log.setDestination(LogDestination::STDOUT);

  GM.startUp();

  auto scene = WM.createScene<Scene>("main");

  auto floor = scene->createObject<Object>();
  floor->setBox({WINDOW_WIDTH*100, 3000});
  floor->setBodyType(lb::BodyType::STATIC);
  floor->setPosition({0, WINDOW_HEIGHT - 30});
  floor->setBounciness(0);
  floor->setDebug(true);

  auto platform = scene->createObject<Object>();
  platform->setBox({100, 10});
  platform->setBodyType(lb::BodyType::STATIC);
  platform->setPosition({100, WINDOW_HEIGHT - 60});
  platform->setBounciness(0);
  platform->setDebug(true);

  auto player = scene->createObject<Player>();
  player->setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT - 40 });
  scene->activate();

  GM.run();
  return 0;
}