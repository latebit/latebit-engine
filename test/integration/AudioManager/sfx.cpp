#include "latebit/core/GameManager.h"
#include "latebit/core/ResourceManager.h"
#include "latebit/core/audio/AudioManager.h"
#include "latebit/core/audio/Music.h"
#include "latebit/core/audio/Sound.h"
#include "latebit/core/events/EventStep.h"
#include "latebit/core/objects/Object.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
  Sound *sound;
  Sound *sound2;
  Sound *sound3;
  Sound *sound4;

 public:
  TestObject() {
    setType("TestObject");
    subscribe(STEP_EVENT);
    auto music = RM.getMusic("song");
    music->play(true);
    sound = RM.getSound("sound");
    sound2 = RM.getSound("sound2");
    sound3 = RM.getSound("sound3");
    sound4 = RM.getSound("sound4");
  }

  int eventHandler(const Event *event) override {
    if (event->getType() == STEP_EVENT) {
      const auto stepEvent = dynamic_cast<const EventStep *>(event);
      const auto step = stepEvent->getStepCount();
      if (step % 60 == 0) {
        sound->play();
      }
      if (step % 120 == 0) {
        sound2->play();
      }
      if (step % 240 == 0) {
        sound3->play();
      }
      if (step % 480 == 0) {
        sound4->play();
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

  if (RM.loadSound("../../test/fixtures/sfx2.lbsfx", "sound") != 0) {
    return 1;
  }

  if (RM.loadSound("../../test/fixtures/sfx.lbsfx", "sound2") != 0) {
    return 1;
  }

  if (RM.loadSound("../../test/fixtures/sfx2.lbsfx", "sound3") != 0) {
    return 1;
  }

  if (RM.loadSound("../../test/fixtures/sfx.lbsfx", "sound4") != 0) {
    return 1;
  }

  new TestObject();
  GM.run();

  return 0;
}