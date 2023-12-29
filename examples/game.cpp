#include "../src/LogManager.h"

int main() {
  LM.writeLog("test");
  LM.writeLog("test");
  LM.writeLog("test %s %d", "test", 10);
  LM.writeLog("test");
  return 0;
}
