#include "Scene.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "test/lib/test.h"

using namespace lb;

void createObject() {
  WM.startUp();
  auto scene = WM.createScene<Scene>("scene");
  scene->createObject<Object>();
  scene->activate();

  assertEq("scene contains the object", scene->getObjects().size(), 1);
  auto objects = WM.getAllObjects();
  assertEq("world manager contains the object", objects.size(), 1);
  WM.shutDown();
}

void removeObject() {
  WM.startUp();
  auto scene = WM.createScene<Scene>("scene");
  auto subject = scene->createObject<Object>();
  scene->activate();

  scene->removeObject(subject);

  assertEq("scene does not contain the object", scene->getObjects().size(), 0);
  auto objects = WM.getAllObjects();
  assertEq("world manager does not contain the object", objects.size(), 0);
  WM.shutDown();
}

void activate() {
  WM.startUp();

  static int onActivatedInvocations = 0;
  class TestScene : public Scene {
   public:
    bool activated = false;
    void onActivated() override { onActivatedInvocations++; }
  };

  auto scene = WM.createScene<TestScene>("scene");
  scene->activate();

  assert("scene is active", scene->isActive());
  assertEq("onActivated is invoked", onActivatedInvocations, 1);
  WM.shutDown();
}

void deactivate() {
  WM.startUp();

  static int onDeactivatedInvocations = 0;
  class TestScene : public Scene {
   public:
    bool activated = false;
    void onDeactivated() override { onDeactivatedInvocations++; }
  };

  auto scene = WM.createScene<TestScene>("scene");
  scene->deactivate();

  assert("scene is not active", scene->isActive());
  assertEq("onDeactivated is invoked", onDeactivatedInvocations, 1);
  WM.shutDown();
}

auto main() -> int {
  test("removeObject", removeObject);
  test("activate", activate);
  test("deactivate", deactivate);
  test("createObject", createObject);

  return report();
}