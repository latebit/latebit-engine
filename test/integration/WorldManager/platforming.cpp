#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/ui/Rectangle.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class Player : public Object {
 public:
  Player() {
    setAcceleration({0, 0.1});
    setSolidness(Solidness::Solidness::HARD);
    subscribe(INPUT_EVENT);
    setBox({ 10, 10 });
    setDebug(true);
    setMass(0.01);
  }

  auto eventHandler(const Event *e) -> int override {
    if (e->getType() == INPUT_EVENT) {
      auto input = static_cast<const EventInput *>(e);
      if (input->getAction() == lb::InputAction::PRESSED &&
          input->getKey() == lb::InputKey::A) {
        setVelocity({0, -1});
      }
    }

    return 0;
  }
};

auto main() -> int {
  Log.setDestination(LogDestination::STDOUT);

  GM.startUp();

  auto scene = WM.createScene<Scene>("main");

  auto floor = scene->createObject<lbui::Rectangle>(lbui::RectangleOptions{
    .height = 30, .width = WINDOW_WIDTH, .fill = Color::BROWN});
    floor->setSolidness(lb::Solidness::HARD);
  floor->setPosition({0, WINDOW_HEIGHT - 30});
  floor->setBounciness(0);
  floor->setMass(1000);
  floor->setDebug(true);

  auto player = scene->createObject<Player>();
  player->setPosition({WINDOW_WIDTH / 2, WINDOW_HEIGHT - 60 });

  scene->activate();
  GM.run();
  return 0;
}