#include <thread>

#include "../src/GameManager.h"
#include "../src/LogManager.h"
#include "../src/utils.h"

int main() {
  LM.setFlush(true);

  GM.startUp();
  GM.run();

  return 0;
}
