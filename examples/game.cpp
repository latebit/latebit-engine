#include <thread>

#include "../src/EventStep.h"
#include "../src/GameManager.h"
#include "../src/LogManager.h"
#include "../src/Object.h"
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
      printf("Enemy %d receives Step\n", getId());
      return 1;
    } else if (p_e->getType() == "CustomEvent") {
      printf("Enemy %d receives CustomEvent\n", getId());
      return 1;
    }
    return 0;
  }
};

int main() {
  LM.setFlush(true);

  // GM.startUp();
  // GM.run();

  Enemy *enemies[5] = {
      new Enemy, new Enemy, new Enemy, new Enemy, new Enemy,
  };

  const df::EventStep *step = new df::EventStep;
  const CustomEvent *custom = new CustomEvent;
  for (int i = 0; i < 5; i++) {
    enemies[i]->eventHandler(step);
    enemies[i]->eventHandler(custom);
  }

  return 0;
}
