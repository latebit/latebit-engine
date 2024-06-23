#include "latebit/core/audio/Music.h"

#include "../../../test/lib/test.h"
#include "latebit/core/audio/AudioManager.h"

auto main() -> int {
  AM.startUp();

  test("constructor", []() {
    unique_ptr<Tune> tune = make_unique<Tune>();
    Music music("test", std::move(tune));
    assertEq("initializes the label", music.getLabel(), "test");
  });

  AM.shutDown();
  return report();
}