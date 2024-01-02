#include <thread>

#include "../src/Colors.h"
#include "../src/DisplayManager.h"
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
  int count;
  Enemy() {
    setType("Enemy");
    count = 0;
  }

  int eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::STEP_EVENT) {
      count++;

      if (count % 30 == 0) {
        auto p = getPosition();
        setPosition(df::Vector(p.getX() + 1, p.getY()));
      }

      return 1;
    } else if (p_e->getType() == "CustomEvent") {
      printf("Enemy %d receives CustomEvent\n", getId());
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

  new Enemy;
  new Star;

  GM.run();

  return 0;
}
