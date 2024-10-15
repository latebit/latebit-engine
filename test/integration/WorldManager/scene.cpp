#include "latebit/core/world/Scene.h"
#include <memory>
#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/Keyframe.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/objects/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
  private:
    unique_ptr<Sprite> sprite;
    string sceneToActivate;
  public:
    TestObject(Vector position, Color::Color color, const string sceneToActivate) : Object("TestObject") {
      const auto size = 5;
      setPosition(position);
      auto animation = Animation();
      Keyframe colors(size * size, color);
      sprite = make_unique<Sprite>("test", size, size, 1, vector<Keyframe>{colors});
      animation.setSprite(sprite.get());
      setAnimation(animation);
      setBox({(float)size, (float)size});
      this->sceneToActivate = sceneToActivate;
      subscribe(INPUT_EVENT);
    }

    auto eventHandler(const Event* evt) -> int override {
      if (evt->getType() == INPUT_EVENT) {
        auto event = static_cast<const EventInput*>(evt);
        if (event->getAction() == lb::InputAction::PRESSED) {
          WM.switchToScene(sceneToActivate);
          return 1;
        }
        return 0;
      }
      return 0;
    }
};

class TestScene: public Scene {
  public:
    TestScene(Color::Color color, Vector position, const string other): Scene() {
      WM.createObject<TestObject>(this, position, color, other);
    }

    void onActivated() override {
      this->objects[0]->setActive(true);
    }
    
    void onDeactivated() override {
      this->objects[0]->setActive(false);
    }
};

auto main() -> int {
  Log.setDestination(LogDestination::STDOUT);
  Log.setLevel(LogLevel::DEBUG);
  GM.startUp();

  WM.createScene<TestScene>("main", Color::RED, Vector(10, 10), "other");
  WM.createScene<TestScene>("other", Color::GREEN, Vector(10, 20), "main");
  WM.activateScene("main");

  GM.run();
  return 0;
}