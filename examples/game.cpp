#include <thread>

#include "../includes/Colors.h"
#include "../includes/DisplayManager.h"
#include "../includes/EventKeyboard.h"
#include "../includes/EventOut.h"
#include "../includes/EventStep.h"
#include "../includes/GameManager.h"
#include "../includes/LogManager.h"
#include "../includes/Object.h"
#include "../includes/Vector.h"
#include "../includes/WorldManager.h"
#include "../includes/utils.h"

class CustomEvent : public df::Event {
 public:
  CustomEvent() { setType("CustomEvent"); }
};

class Enemy : public df::Object {
 public:
  Enemy() {
    setType("Enemy");
    setAltitude(df::MAX_ALTITUDE - 1);
  }

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
    return DM.drawString(df::Vector(1, 0), "Snow", df::ALIGN_LEFT,
                         df::Color::RED);
  }
};

int getRandom(int max, int min = 0) {
  int delta = max - min;
  return min + rand() % (delta + 1);
}

class Flake : public df::Object {
 private:
  void resetPositionAndVelocity() {
    setPosition(
        df::Vector(getRandom(DM.getHorizontalCells()), -1 * getRandom(5, 1)));
    setVelocity(df::Vector(0.1, 0.05 * getRandom(6)));
    m_step_check = getRandom(120, 1);
  }

  int m_step_check = 1;

 public:
  Flake() {
    resetPositionAndVelocity();
    setSolidness(df::SPECTRAL);
    setType("Flake");
  }

  int draw() {
    return DM.drawCh(getPosition(), '*', df::Color::WHITE,
                     df::Color::UNDEFINED_COLOR);
  }

  int eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::OUT_EVENT) {
      resetPositionAndVelocity();
      return 1;
    }
    if (p_e->getType() == df::STEP_EVENT) {
      auto p_event_step = dynamic_cast<const df::EventStep *>(p_e);

      auto steps = p_event_step->getStepCount();
      if (steps % m_step_check == 0) {
        df::Vector v = getVelocity();
        float x = v.getX() * -1;
        setVelocity(df::Vector(x, v.getY()));
      };

      return 1;
    }
    return 0;
  }
};

int main() {
  LM.setFlush(true);

  GM.startUp();

  new Enemy;

  for (int i = 0; i < 1000; i++) {
    new Flake;
  }

  GM.run();

  return 0;
}
