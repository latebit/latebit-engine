#include <thread>

#include "../src/Colors.h"
#include "../src/DisplayManager.h"
#include "../src/EventKeyboard.h"
#include "../src/EventStep.h"
#include "../src/GameManager.h"
#include "../src/LogManager.h"
#include "../src/Object.h"
#include "../src/Vector.h"
#include "../src/utils.h"

class CustomEvent : public df::Event {
 public:
  CustomEvent() { setType("CustomEvent"); }
};

class Enemy : public df::Object {
 public:
  Enemy() { setType("Enemy"); }

  int eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::STEP_EVENT) {
      return 1;
    } else if (p_e->getType() == df::KEYBOARD_EVENT) {
      auto e = (df::EventKeyboard *)p_e;
      if (e->getKeyboardAction() == df::EventKeyboardAction::KEY_PRESSED) {
        if (e->getKey() == df::Keyboard::LEFTARROW) {
          setPosition(getPosition() + df::Vector(-1, 0));
        } else if (e->getKey() == df::Keyboard::RIGHTARROW) {
          setPosition(getPosition() + df::Vector(1, 0));
        }
      }
      return 1;
    }
    return 0;
  }

  int draw() { return DM.drawCh(getPosition(), '1', df::Color::RED); }
};

class Star : public df::Object {
 public:
  Star() {
    setType("Star");
    setAltitude(0);
    setPosition(df::Vector(3, 0));
  }

  int draw() { return DM.drawCh(getPosition(), '*', df::Color::YELLOW); }
};

int main() {
  LM.setFlush(true);

  GM.startUp();
  DM.setBackground(df::Color::CYAN);

  new Enemy;
  new Star;

  GM.run();

  return 0;
}
