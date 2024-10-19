#include "Scene.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"
#include "test/lib/test.h"

using namespace lb;

void addObject() {
  WM.startUp();
  auto scene = WM.createScene<Scene>("scene");
  scene->activate();
  auto obj = make_unique<Object>();

  scene->addObject(std::move(obj));

  assertEq("scene contains the object", scene->getObjects().size(), 1);
  auto objects = WM.getAllObjects();
  assertEq("world manager contains the object", objects.size(), 1);
  WM.shutDown();
}

void removeObject() {
  WM.startUp();
  auto scene = WM.createScene<Scene>("scene");
  scene->activate();
  auto obj = make_unique<Object>();
  auto subject = obj.get();

  scene->addObject(std::move(obj));
  scene->removeObject(subject);

  assertEq("scene does not contain the object", scene->getObjects().size(), 0);
  auto objects = WM.getAllObjects();
  assertEq("world manager does not contain the object", objects.size(), 0);
  WM.shutDown();
}

void activate() {
  WM.startUp();
  auto scene = WM.createScene<Scene>("scene");
  auto obj = make_unique<Object>();

  scene->activate();

  assert("scene is active", scene->isActive());
  WM.shutDown();
}

void deactivate() {
  WM.startUp();
  auto scene = WM.createScene<Scene>("scene");
  auto obj = make_unique<Object>();

  scene->addObject(std::move(obj));
  scene->activate();
  scene->deactivate();

  assert("scene is not active", !scene->isActive());
  WM.shutDown();
}

auto main() -> int {
  test("addObject", addObject);
  test("removeObject", removeObject);
  test("activate", activate);
  test("deactivate", deactivate);

  return report();
}