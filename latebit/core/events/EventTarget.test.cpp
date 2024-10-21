#include <unordered_map>

#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventCollision.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/events/EventOut.h"
#include "latebit/core/events/EventStep.h"
#include "latebit/core/events/EventTarget.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/core/input/InputManager.h"
#include "test/lib/test.h"

void whenActive() {
  static unordered_map<string, int> emittedCount = {};
  EventTarget subject;

  class TestEventTarget : public EventTarget {
   public:
    TestEventTarget() : EventTarget(){};
    auto eventHandler(const Event* e) -> int override {
      emittedCount[e->getType()]++;
      return 0;
    };
    [[nodiscard]] auto isActive() const -> bool override { return true; };
  };

  TestEventTarget obj;

  assertOk("subscribes to Collision", obj.subscribe(COLLISION_EVENT));
  assertOk("subscribes to Out", obj.subscribe(OUT_EVENT));
  assertOk("subscribes to Step", obj.subscribe(STEP_EVENT));
  assertOk("subscribes to Input", obj.subscribe(INPUT_EVENT));
  assertOk("subscribes to custom", obj.subscribe("custom"));

  EventCollision collision;
  WM.onEvent(&collision);
  assertEq("responds to Collision", emittedCount[COLLISION_EVENT], 1);
  EventOut out;
  WM.onEvent(&out);
  assertEq("responds to Out", emittedCount[OUT_EVENT], 1);

  EventStep step;
  GM.onEvent(&step);
  assertEq("responds to Step", emittedCount[STEP_EVENT], 1);

  EventInput input;
  IM.onEvent(&input);
  assertEq("responds to Keyboard", emittedCount[INPUT_EVENT], 1);

  Event customEvent("custom");
  WM.onEvent(&customEvent);
  assertEq("responds to custom event", emittedCount["custom"], 1);

  assertOk("unsubscribes to Collision", obj.unsubscribe(COLLISION_EVENT));
  assertOk("unsubscribes to Out", obj.unsubscribe(OUT_EVENT));
  assertOk("unsubscribes to Step", obj.unsubscribe(STEP_EVENT));
  assertOk("unsubscribes to Input", obj.unsubscribe(INPUT_EVENT));
  assertOk("unsubscribes to custom", obj.unsubscribe("custom"));

  WM.onEvent(&collision);
  assertEq("does not respond to Collision", emittedCount[COLLISION_EVENT], 1);

  WM.onEvent(&out);
  assertEq("does not respond to Out", emittedCount[OUT_EVENT], 1);

  GM.onEvent(&step);
  assertEq("does not respond to Step", emittedCount[STEP_EVENT], 1);

  IM.onEvent(&input);
  assertEq("does not respond to Keyboard", emittedCount[INPUT_EVENT], 1);

  WM.onEvent(&customEvent);
  assertEq("does not respond to custom event", emittedCount["custom"], 1);
}

void whenInactive() {
  static unordered_map<string, int> emittedCount = {};
  EventTarget subject;

  class TestEventTarget : public EventTarget {
   public:
    TestEventTarget() : EventTarget(){};

    auto eventHandler(const Event* e) -> int override {
      emittedCount[e->getType()]++;
      return 0;
    };

    [[nodiscard]] auto isActive() const -> bool override { return false; };
  };

  TestEventTarget obj;

  assertOk("subscribes to Collision", obj.subscribe(COLLISION_EVENT));
  assertOk("subscribes to Out", obj.subscribe(OUT_EVENT));
  assertOk("subscribes to Step", obj.subscribe(STEP_EVENT));
  assertOk("subscribes to Input", obj.subscribe(INPUT_EVENT));
  assertOk("subscribes to custom", obj.subscribe("custom"));

  EventCollision collision;
  WM.onEvent(&collision);
  assertEq("does not respond to Collision", emittedCount[COLLISION_EVENT], 0);
  EventOut out;
  WM.onEvent(&out);
  assertEq("does not respond to Out", emittedCount[OUT_EVENT], 0);

  EventStep step;
  GM.onEvent(&step);
  assertEq("does not respond to Step", emittedCount[STEP_EVENT], 0);

  EventInput input;
  IM.onEvent(&input);
  assertEq("does not respond to Keyboard", emittedCount[INPUT_EVENT], 0);

  Event customEvent("custom");
  WM.onEvent(&customEvent);
  assertEq("does not respond to custom event", emittedCount["custom"], 0);

  assertOk("unsubscribes to Collision", obj.unsubscribe(COLLISION_EVENT));
  assertOk("unsubscribes to Out", obj.unsubscribe(OUT_EVENT));
  assertOk("unsubscribes to Step", obj.unsubscribe(STEP_EVENT));
  assertOk("unsubscribes to Input", obj.unsubscribe(INPUT_EVENT));
  assertOk("unsubscribes to custom", obj.unsubscribe("custom"));

  WM.onEvent(&collision);
  assertEq("does not respond to Collision", emittedCount[COLLISION_EVENT], 0);

  WM.onEvent(&out);
  assertEq("does not respond to Out", emittedCount[OUT_EVENT], 0);

  GM.onEvent(&step);
  assertEq("does not respond to Step", emittedCount[STEP_EVENT], 0);

  IM.onEvent(&input);
  assertEq("does not respond to Keyboard", emittedCount[INPUT_EVENT], 0);

  WM.onEvent(&customEvent);
  assertEq("does not respond to custom event", emittedCount["custom"], 0);
}

auto main() -> int {
  test("when active", whenActive);
  test("when inactive", whenInactive);
  return report();
}