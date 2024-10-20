#include "../../../test/lib/test.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/WorldManager.h"

using namespace std;
using namespace lb;

void worldToView() {
  auto initialView = WM.getView().getView();
  WM.getView().setView(Box(Vector(5, 5), 10, 10));

  assertEq("converts world position to view position",
           WM.getView().worldToView(Vector(5, 5)), Vector(0, 0));
  assertEq("converts world position to view position (origin)",
           WM.getView().worldToView(Vector(0, 0)), Vector(-5, -5));
  WM.getView().setView(initialView);
}

void viewToWorld() {
  auto initialView = WM.getView().getView();
  WM.getView().setView(Box(Vector(5, 5), 10, 10));

  assertEq("converts view position to world position",
           WM.getView().viewToWorld(Vector(0, 0)), Vector(5, 5));
  assertEq("converts view position to world position (origin)",
           WM.getView().worldToView(Vector(-5, -5)), Vector(0, 0));
  WM.getView().setView(initialView);
}

void viewFollowing() {
  WM.startUp();

  auto scene = WM.createScene<Scene>("main");
  auto subject = scene->createObject<Object>();
  subject->setPosition(Vector());
  scene->activate();

  auto initialView = Box(Vector(5, 5), 10, 10);
  WM.getView().setView(initialView);
  WM.setBoundary(Box(20, 20));

  WM.getView().setViewFollowing(subject);
  WM.resolveMovement(subject, Vector(10, 10));
  assertEq("does not update view", WM.getView().getView(), initialView);
  WM.resolveMovement(subject, Vector(11, 11));

  assertEq("updates the view", WM.getView().getView(),
           Box(Vector(6, 6), 10, 10));

  WM.resolveMovement(subject, Vector(11, 5));
  assertEq("updates the view (vertical lower bound)", WM.getView().getView(),
           Box(Vector(6, 0), 10, 10));

  WM.resolveMovement(subject, Vector(11, 15));
  assertEq("updates the view (vertical upper bound)", WM.getView().getView(),
           Box(Vector(6, 10), 10, 10));

  WM.resolveMovement(subject, Vector(5, 11));
  assertEq("updates the view (horizontal lower bound)", WM.getView().getView(),
           Box(Vector(0, 6), 10, 10));

  WM.resolveMovement(subject, Vector(15, 11));
  assertEq("updates the view (horizontal upper bound)", WM.getView().getView(),
           Box(Vector(10, 6), 10, 10));

  WM.getView().setViewDeadZone(Box(WM.getView().getView().getCorner(), 5, 5));
  WM.resolveMovement(subject, Vector(12, 10));
  assertEq("does not update the view within dead zone", WM.getView().getView(),
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
