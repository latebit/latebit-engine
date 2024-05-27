#include "latebit/core/GameManager.h"
#include "latebit/core/ResourceManager.h"
#include "latebit/core/audio/AudioManager.cpp"
#include "latebit/core/audio/Music.h"
#include "latebit/core/audio/Sound.h"
#include "latebit/core/events/EventStep.h"
#include "latebit/core/objects/Object.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
 public:
  TestObject() {
    setType("TestObject");
    auto music = RM.getMusic("song");
    music->play(true);
    subscribe(STEP_EVENT);
  }

  int eventHandler(const Event *e) {
    if (e->getType() == STEP_EVENT) {
      auto event = static_cast<const EventStep *>(e);
      int i = rand() % 50 + 10;
      if (event->getStepCount() % i == 0) {
        auto sound = RM.getSound("sound");
        sound->play();
      }
    }
    return 0;
  }
};

int main() {
  Log.setDestination(STDOUT);
  GM.startUp();

  if (RM.loadMusic("../../test/fixtures/song.lbtune", "song") != 0) {
    return 1;
  }

  if (RM.loadSound("../../test/fixtures/sfx.lbtune", "sound") != 0) {
    return 1;
  }

  new TestObject();
  GM.run();

  return 0;
}