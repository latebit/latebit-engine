#include "../include/Manager.h"

#include <string>

#include "../include/Object.h"
#include "test.h"

const string Manager_test_evt = "TestEvent";
const string Manager_test_wrongEvt = "TestEvent2";
int Manager_test_emittedCount = 0;

auto Manager_test() -> int {
  int result = 0;

  class TestManager : public df::Manager {
   public:
    TestManager() { this->setType("TestType"); };
    [[nodiscard]] auto isValid(string eventType) const -> bool override {
      return eventType == Manager_test_evt;
    };
  };

  // Test setType() and getType()
  TestManager manager;
  result += assert_string("sets correct type", manager.getType(), "TestType");

  // Test isStarted()
  result +=
    assert("isStarted() returns false by default", !manager.isStarted());

  // Test startUp() and isStarted()
  result += assert_ok("starts up", manager.startUp());
  result += assert("sets isStarted", manager.isStarted());

  // Test shutDown() and isStarted()
  manager.shutDown();
  result += assert("updates isStarted after shutdown", !manager.isStarted());

  class TestObject : public df::Object {
   public:
    TestObject() { this->setType("TestObject"); };
    auto eventHandler(const df::Event* e) -> int override {
      if (e->getType() == Manager_test_evt) {
        Manager_test_emittedCount++;
        return 0;
      }
      return -1;
    };
  };

  TestObject obj;
  result += assert_ok("subscribes to TestEvent",
                      manager.subscribe(&obj, Manager_test_evt));
  result += assert_fail("cannot subscribe to another event",
                        manager.subscribe(&obj, Manager_test_wrongEvt));

  result += assert_ok("unsubscribes from TestEvent",
                      manager.unsubscribe(&obj, Manager_test_evt));

  auto event = df::Event();
  manager.onEvent(&event);
  manager.subscribe(&obj, Manager_test_evt);
  result +=
    assert_int("does not trigger for wrong type", Manager_test_emittedCount, 0);
  event.setType(Manager_test_evt);
  manager.onEvent(&event);
  result +=
    assert_int("triggers for correct type", Manager_test_emittedCount, 1);

  result +=
    assert("returns true for valid events", manager.isValid(Manager_test_evt));
  result += assert("returns false for wrong events",
                   !manager.isValid(Manager_test_wrongEvt));

  return result;
}