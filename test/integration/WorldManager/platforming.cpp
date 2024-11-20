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
    subscribe(INPUT_EVENT);
    subscribe(STEP_EVENT);
    setBox({{4, 4}, 10, 10});
    setDebug(true);
    setSolidness(lb::Solidness::HARD);
    setType("Player");
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
    switch (key) {
      case InputKey::A:
        if (getVelocity().getY() == 0) {
          setVelocity({getVelocity().getX(), -3});
          setAcceleration({0, 0.1});
        }
        return 1;
      case InputKey::LEFT:
        setVelocity({-1, getVelocity().getY()});
        setAcceleration({0, 0.1});
        return 1;
      case InputKey::RIGHT:
        setVelocity({1.2, getVelocity().getY()});
        setAcceleration({0, 0.1});
        return 1;
      default:
        return 0;
    }
  }

  auto onInputReleased(const InputKey::InputKey key) -> int {
    switch (key) {
      case InputKey::RIGHT:
        if (getVelocity().getX() > 0) {
          setAcceleration({-0.1, getAcceleration().getY()});
        }
        stopAtZero = true;
        return 1;
      case InputKey::LEFT:
        if (getVelocity().getX() < 0) {
          setAcceleration({0.1, getAcceleration().getY()});
        }
        stopAtZero = true;
        return 1;
      default:
        return 0;
    }
  }

  auto onStep(const EventStep *) -> int {
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

  auto wall = scene->createObject<Object>();
  wall->setBox({10, WINDOW_HEIGHT - 30});
  wall->setPosition({0, 0});
  wall->setDebug(true);
  wall->setSolidness(Solidness::HARD);
  wall->setType("Wall");

  auto wall2 = scene->createObject<Object>();
  wall2->setBox({10, WINDOW_HEIGHT - 30});
  wall2->setPosition({WINDOW_WIDTH-10, 0});
  wall2->setDebug(true);
  wall2->setSolidness(Solidness::HARD);
  wall2->setType("Wall");

  auto floor = scene->createObject<Object>();
  floor->setBox({WINDOW_WIDTH * 100, 3000});
  floor->setPosition({0, WINDOW_HEIGHT - 30});
  floor->setDebug(true);
  floor->setSolidness(Solidness::HARD);
  floor->setType("Floor");

  auto platform = scene->createObject<Object>();
  platform->setBox({100, 10});
  platform->setPosition({100, WINDOW_HEIGHT - 55});
  platform->setDebug(true);
  floor->setSolidness(Solidness::HARD);
  platform->setType("Platform");

  auto prop = scene->createObject<Object>();
  prop->setBox({10, 10});
  prop->setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2});
  prop->setDebug(true);
  prop->setSolidness(Solidness::SPECTRAL);

  auto player = scene->createObject<Player>();
  player->setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT - 60});
  scene->activate();

  GM.run();
  return 0;
}