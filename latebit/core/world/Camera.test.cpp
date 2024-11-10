#include "../../../test/lib/test.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/world/Object.h"
#include "latebit/core/world/Physics.h"
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

void moveTo(Physics& physics, Camera& camera, Object* o, const Vector p) {
  physics.moveTo(o, p);
  camera.update();
};

void viewFollowing() {
  WM.startUp();
  auto& physics = WM.getPhysics();

  auto scene = WM.createScene<Scene>("main");
  auto subject = scene->createObject<Object>();
  subject->setPosition(Vector());
  scene->activate();

  auto initialView = Box(Vector(5, 5), 10, 10);
  auto& camera = WM.getCamera();
  camera.setView(initialView);
  WM.setBoundary(Box(20, 20));

  camera.setViewFollowing(subject);

  moveTo(physics, camera, subject, {10, 10});
  assertEq("does not update view", camera.getView(), initialView);

  moveTo(physics, camera, subject, {11, 11});
  assertEq("updates the view", camera.getView(), Box(Vector(6, 6), 10, 10));

  moveTo(physics, camera, subject, {11, 5});
  assertEq("updates the view (vertical lower bound)", camera.getView(),
           Box(Vector(6, 0), 10, 10));

  moveTo(physics, camera, subject, {11, 15});
  assertEq("updates the view (vertical upper bound)", camera.getView(),
           Box(Vector(6, 10), 10, 10));

  moveTo(physics, camera, subject, {5, 11});
  assertEq("updates the view (horizontal lower bound)", camera.getView(),
           Box(Vector(0, 6), 10, 10));

  moveTo(physics, camera, subject, {15, 11});
  assertEq("updates the view (horizontal upper bound)", camera.getView(),
           Box(Vector(10, 6), 10, 10));

  camera.setViewDeadZone(Box(camera.getView().getCorner(), 5, 5));
  moveTo(physics, camera, subject, {12, 10});
  assertEq("does not update the view within dead zone", camera.getView(),
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
