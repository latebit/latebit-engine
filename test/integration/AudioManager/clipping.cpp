#include "latebit/core/GameManager.h"
#include "latebit/core/ResourceManager.h"
#include "latebit/core/audio/Music.h"
#include "latebit/core/audio/Sound.h"
#include "latebit/core/objects/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"

using namespace lb;

class TestObject : public Object {
 public:
  TestObject() {
    setType("TestObject");
    auto music = RM.getMusic("song");
    music->play(true);
    auto sound = RM.getSound("sound");
    sound->play(true);
  }
};

int main() {
  Log.setDestination(LogDestination::STDOUT);
  GM.startUp();

  if (RM.loadMusic("../../test/fixtures/song.lbtune", "song") != 0) {
    return 1;
  }

  if (RM.loadSound("../../test/fixtures/sfx.lbsfx", "sound") != 0) {
    return 1;
  }

  WM.create<TestObject>();
  GM.run();

  return 0;
}