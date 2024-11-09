#include "../../../test/lib/test.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"

using namespace std;
using namespace lb;

void worldToView() {
  auto initialView = WM.getCamera().getView();
  WM.getCamera().setView(Box(Vector(5, 5), 10, 10));

  assertEq("converts world position to view position",
           WM.getCamera().worldToView(Vector(5, 5)), Vector(0, 0));
  assertEq("converts world position to view position (origin)",
           WM.getCamera().worldToView(Vector(0, 0)), Vector(-5, -5));
  WM.getCamera().setView(initialView);
}

void viewToWorld() {
  auto initialView = WM.getCamera().getView();
  WM.getCamera().setView(Box(Vector(5, 5), 10, 10));

  assertEq("converts view position to world position",
           WM.getCamera().viewToWorld(Vector(0, 0)), Vector(5, 5));
  assertEq("converts view position to world position (origin)",
           WM.getCamera().worldToView(Vector(-5, -5)), Vector(0, 0));
  WM.getCamera().setView(initialView);
}

void viewFollowing() {
  WM.startUp();

  auto scene = WM.createScene<Scene>("main");
  auto subject = scene->createObject<Object>();
  subject->setPosition(Vector());
  scene->activate();

  auto initialView = Box(Vector(5, 5), 10, 10);
  WM.getCamera().setView(initialView);
  WM.setBoundary(Box(20, 20));

  WM.getCamera().setViewFollowing(subject);
  WM.updatePhysics();
  assertEq("does not update view", WM.getCamera().getView(), initialView);
  WM.updatePhysics();

  assertEq("updates the view", WM.getCamera().getView(),
           Box(Vector(6, 6), 10, 10));

  WM.updatePhysics();
  assertEq("updates the view (vertical lower bound)", WM.getCamera().getView(),
           Box(Vector(6, 0), 10, 10));

  WM.updatePhysics();
  assertEq("updates the view (vertical upper bound)", WM.getCamera().getView(),
           Box(Vector(6, 10), 10, 10));

  WM.updatePhysics();
  assertEq("updates the view (horizontal lower bound)", WM.getCamera().getView(),
           Box(Vector(0, 6), 10, 10));

  WM.updatePhysics();
  assertEq("updates the view (horizontal upper bound)", WM.getCamera().getView(),
           Box(Vector(10, 6), 10, 10));

  WM.getCamera().setViewDeadZone(Box(WM.getCamera().getView().getCorner(), 5, 5));
  WM.updatePhysics();
  assertEq("does not update the view within dead zone", WM.getCamera().getView(),
           Box(Vector(10, 6), 10, 10));

  WM.shutDown();
}

auto main() -> int {
  WM.startUp();
  test("viewFollowing", viewFollowing);
  test("worldToView", worldToView);
  test("viewToWorld", worldToView);
  WM.shutDown();
  return report();
}
