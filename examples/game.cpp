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

  int draw() {
    return DM.drawString(df::Vector(1, 0),
                         "11111111111111111111111111111111111111111111111111111"
                         "11111111111111111111111111",
                         df::ALIGN_LEFT, df::Color::RED);
  }
};

class Flake : public df::Object {
 public:
  Flake() {
    setType("Flake");
    setAltitude(0);
    setPosition(
        df::Vector(rand() % (DM.getHorizontalCells() + 1),
                   rand() % DM.getVerticalCells() - DM.getVerticalCells() / 2));
    setVelocity(df::Vector(0, 0.1));
  }

  int draw() { return DM.drawCh(getPosition(), '*', df::Color::YELLOW); }
};

int main() {
  LM.setFlush(true);

  GM.startUp();

  new Enemy;

  for (int i = 0; i < 10000; i++) {
    new Flake;
  }

  GM.run();

  return 0;
}
