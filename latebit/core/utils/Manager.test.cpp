#include "latebit/core/utils/Manager.h"

#include <string>

#include "latebit/core/world/Object.h"
#include "test/lib/test.h"

const string MANAGER_TEST_EVENT = "TestEvent";
const string MANAGER_TEST_ANOTHER_EVENT = "TestEvent2";

int emittedCount = 0;
auto main() -> int {
  class TestManager : public Manager {
   public:
    TestManager() : Manager("TestType") {};
    [[nodiscard]] auto isValid(string eventType) const -> bool override {
      return eventType == MANAGER_TEST_EVENT;
    };
  };

  class TestObject : public Object {
   public:
    TestObject() : Object("TestObject") {};
    auto eventHandler(const Event* e) -> int override {
      if (e->getType() == MANAGER_TEST_EVENT) {
        emittedCount++;
        return 0;
      }
      return -1;
    };
  };

  test("constructor", []() {
    TestManager manager;
    assertEq("sets correct type", manager.getType(), "TestType");
    assert("isStarted() returns false by default", !manager.isStarted());
  });

  test("startUp/shutDown", []() {
    TestManager manager;
    assertOk("starts up", manager.startUp());
    assert("sets isStarted", manager.isStarted());

    manager.shutDown();
    assert("updates isStarted after shutdown", !manager.isStarted());
  });

  test("subscribe/unsubscribe", []() {
    TestManager manager;
    TestObject obj;

    assertOk("subscribes to TestEvent",
             manager.subscribe(&obj, MANAGER_TEST_EVENT));
    assertFail("cannot subscribe to another event",
               manager.subscribe(&obj, MANAGER_TEST_ANOTHER_EVENT));

    assertOk("unsubscribes from TestEvent",
             manager.unsubscribe(&obj, MANAGER_TEST_EVENT));

    auto event = Event();

    manager.subscribe(&obj, MANAGER_TEST_EVENT);
    manager.onEvent(&event);
    assertEq("does not trigger for wrong type", emittedCount, 0);

    auto withCorrectType = Event(MANAGER_TEST_EVENT);

    assertOk("makes the object inactive", obj.setActive(false));
    manager.onEvent(&withCorrectType);
    assertEq("does not trigger if object is inactive", emittedCount, 0);

    assertOk("makes the object active", obj.setActive(true));
    manager.onEvent(&withCorrectType);
    assertEq("triggers for correct type", emittedCount, 1);

    assert("returns true for valid events",
           manager.isValid(MANAGER_TEST_EVENT));
    assert("returns false for wrong events",
           !manager.isValid(MANAGER_TEST_ANOTHER_EVENT));
  });

  return report();
}