#include <sstream>

#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
  string lastKey = "n/a";
  string lastAction = "n/a";

 public:
  TestObject() : Object("TestObject") { subscribe(INPUT_EVENT); }

  auto eventHandler(const Event *event) -> int override {
    if (event->getType() == INPUT_EVENT) {
      auto e = dynamic_cast<const EventInput *>(event);
      lastKey = keyToChar(e->getKey());
      lastAction = toActionString(e->getAction());
    }
    return 0;
  }

  auto draw() -> int override {
    int result = 0;
    stringstream key;
    key << "Last key: " << this->lastKey << ".";
    stringstream action;
    action << "Last action: " << this->lastAction << ".";

    result +=
      DM::drawString(Vector(20, 20), key.str(), TextAlignment::LEFT, Color::RED);
    result += DM::drawString(Vector(20, 40), action.str(), TextAlignment::LEFT,
                            Color::BLUE);
    return result;
  }

  auto keyToChar(InputKey::InputKey k) -> char {
    switch (k) {
      case InputKey::A:
        return InputGlyph::A;
      case InputKey::B:
        return InputGlyph::B;
      case InputKey::L:
        return InputGlyph::L;
      case InputKey::R:
        return InputGlyph::R;
      case InputKey::START:
        return InputGlyph::START;
      case InputKey::OPTIONS:
        return InputGlyph::OPTIONS;
      case InputKey::UP:
        return InputGlyph::UP;
      case InputKey::DOWN:
        return InputGlyph::DOWN;
      case InputKey::LEFT:
        return InputGlyph::LEFT;
      case InputKey::RIGHT:
        return InputGlyph::RIGHT;
      case InputKey::UNDEFINED_KEY:
        return 0;
    }
    return 0;
  }

  auto toActionString(InputAction::InputAction a) -> string {
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

auto main() -> int {
  Log.setDestination(LogDestination::STDOUT);
  GM.startUp();
  auto scene = WM.createScene<Scene>("main");
  WM.createObject<TestObject>(scene);
  scene->activate();
  GM.run();
  return 0;
}