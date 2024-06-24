#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/objects/Object.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
  string lastKey = "n/a";
  string lastAction = "n/a";

 public:
  TestObject() {
    setType("TestObject");
    subscribe(INPUT_EVENT);
  }

  auto eventHandler(const Event *event) -> int override {
    if (event->getType() == INPUT_EVENT) {
      auto e = dynamic_cast<const EventInput *>(event);
      lastKey = toKeyString(e->getKey());
      lastAction = toActionString(e->getAction());
    }
    return 0;
  }

  auto draw() -> int override {
    int result = 0;
    result += DM.drawString(Vector(20, 20), "Last Key: " + this->lastKey,
                            TextAlignment::LEFT, Color::RED);
    result += DM.drawString(Vector(20, 40), "Last Action: " + this->lastAction,
                            TextAlignment::LEFT, Color::BLUE);
    return result;
  }

  string toKeyString(InputKey::InputKey k) {
    switch (k) {
      case InputKey::A:
        return "A";
      case InputKey::B:
        return "B";
      case InputKey::C:
        return "C";
      case InputKey::D:
        return "D";
      case InputKey::START:
        return "START";
      case InputKey::OPTIONS:
        return "OPTIONS";
      case InputKey::UP:
        return "UP";
      case InputKey::DOWN:
        return "DOWN";
      case InputKey::LEFT:
        return "LEFT";
      case InputKey::RIGHT:
        return "RIGHT";
      case InputKey::UNDEFINED_KEY:
        return "UNDEFINED";
      default:
        return "Wrong Key";
    }
  }

  string toActionString(InputAction::InputAction a) {
    switch (a) {
      case InputAction::PRESSED:
        return "PRESSED";
      case InputAction::RELEASED:
        return "RELEASED";
      case InputAction::UNDEFINED_ACTION:
        return "UNDEFINED";
      default:
        return "Wrong Action";
    }
  }
};

int main() {
  Log.setDestination(LogDestination::STDOUT);
  GM.startUp();
  new TestObject();
  GM.run();
  return 0;
}