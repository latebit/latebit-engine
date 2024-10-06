#include "SceneGraph.h"
#include <memory>

#include "latebit/core/objects/Object.h"
#include "latebit/core/objects/ObjectUtils.h"
#include "test/lib/test.h"

using namespace lb;

void insertObject() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto altitude = obj->getAltitude();
  auto nonSolidObj = make_unique<Object>();
  nonSolidObj->setSolidness(Solidness::SPECTRAL);

  // Check if the object was inserted successfully
  assertOk("inserts object into the scene graph", sg.insertObject(std::move(obj)));
  assertEq("scene graph contains the object", sg.getAllObjects().size(), 1);
  assertEq("scene graph contains the active object", sg.getActiveObjects().size(), 1);
  assertEq("scene graph contains the solid object",
           sg.getSolidObjects().size(), 1);
  assertEq("scene graph contains the visible object",
           sg.getVisibleObjects(altitude).size(), 1);
  assertOk("inserts non solid object into the scene graph",
           sg.insertObject(std::move(nonSolidObj)));
  assertEq("scene graph does not contain the non-solid object",
           sg.getSolidObjects().size(), 1);
}

void removeObject() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  auto altitude = obj->getAltitude();

  // Insert object into the scene graph
  sg.insertObject(std::move(obj));

  // Check if the object was removed successfully
  assertOk("removes object from the scene graph", sg.removeObject(subject));
  assertEq("scene graph does not contain the object",
           sg.getAllObjects().size(), 0);
  assertEq("scene graph does not contain the active object",
           sg.getActiveObjects().size(), 0);
  assertEq("scene graph does not contain the solid object",
           sg.getSolidObjects().size(), 0);
  assertEq("scene graph does not contain the visible object",
           sg.getVisibleObjects(altitude).size(), 0);
}

void getActiveObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(), obj3 = make_unique<Object>(), obj4 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get(), subject4 = obj4.get();
  obj2->setActive(false);

  // Insert objects into the scene graph
  sg.insertObject(std::move(obj1));
  sg.insertObject(std::move(obj2));
  sg.insertObject(std::move(obj3));
  sg.insertObject(std::move(obj4));

  // Check if all objects are returned
  assertEq("returns all objects from the scene graph", sg.getActiveObjects().size(), 3);
  assert("contains object 1", contains(sg.getActiveObjects(), subject1));
  assert("does not contain object 2", !contains(sg.getActiveObjects(), subject2));
  assert("contains object 3", contains(sg.getActiveObjects(), subject3));
  assert("contains object 4", contains(sg.getActiveObjects(), subject4));
}

void getInactiveObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(), obj3 = make_unique<Object>(), obj4 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get(), subject4 = obj4.get();
  obj2->setActive(false);

  sg.insertObject(std::move(obj1));
  sg.insertObject(std::move(obj2));
  sg.insertObject(std::move(obj3));
  sg.insertObject(std::move(obj4));

  auto inactive = sg.getInactiveObjects();

  assertEq("returns inactive objects from the scene graph", inactive.size(), 1);
  assert("does not contain object 1", !contains(inactive, subject1));
  assert("contains object 2", contains(inactive, subject2));
  assert("does not contain object 3", !contains(inactive, subject3));
  assert("does not contain object 4", !contains(inactive, subject4));
}

void getSolidObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(), obj3 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get();
  obj2->setSolidness(Solidness::SPECTRAL);

  sg.insertObject(std::move(obj1));
  sg.insertObject(std::move(obj2));
  sg.insertObject(std::move(obj3));

  auto solid = sg.getSolidObjects();

  assertEq("returns solid objects from the scene graph", solid.size(), 2);
  assert("contains solid object 1", contains(solid, subject1));
  assert("does not contain solid object 2", !contains(solid, subject2));
  assert("contains solid object 3", contains(solid, subject3));
}

void getVisibleObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(), obj3 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get();

  // Set altitudes for the objects
  obj1->setAltitude(0);
  obj2->setAltitude(1);
  obj3->setAltitude(1);

  // Insert objects into the scene graph
  sg.insertObject(std::move(obj1));
  sg.insertObject(std::move(obj2));
  sg.insertObject(std::move(obj3));

  // Get visible objects with altitude 1 from the scene graph
  auto visible = sg.getVisibleObjects(1);

  // Check if visible objects with altitude 1 are returned
  assertEq("returns visible objects with altitude 1 from the scene graph",
           visible.size(), 2);
  assert("contains visible object 2", contains(visible, subject2));
  assert("contains visible object 3", contains(visible, subject3));

  // Get visible objects with altitude 0 from the scene graph
  visible = sg.getVisibleObjects(0);
  
  // Check if visible objects with altitude 0 are returned
  assertEq("returns visible objects with altitude 0 from the scene graph",
           visible.size(), 1);
  assert("contains visible object 1", contains(visible, subject1));
}

void setSolidness() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  sg.insertObject(std::move(obj));

  int result = sg.setSolidness(subject, Solidness::HARD);
  assertEq("sets solidness of the object to HARD", result, 0);
  assertEq("scene graph contains the solid object",
           sg.getSolidObjects().size(), 1);

  result = sg.setSolidness(subject, Solidness::SPECTRAL);
  assertEq("sets solidness of the object to SPECTRAL", result, 0);
  assertEq("scene graph does not contain the solid object",
           sg.getSolidObjects().size(), 0);
}

void setAltitude() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  sg.insertObject(std::move(obj));

  int result = sg.setAltitude(subject, 1);
  assertEq("sets altitude of the object to 1", result, 0);
  assertEq("scene graph contains the visible object",
           sg.getVisibleObjects(1).size(), 1);

  result = sg.setAltitude(subject, -1);
  assertEq("does not set altitude of the object to -1", result, -1);
  assertEq("scene graph does not contain the visible object",
           sg.getVisibleObjects(-1).size(), 0);
}

void setVisible() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  auto altitude = obj->getAltitude();
  sg.insertObject(std::move(obj));

  // The following assertions are order dependent
  assertOk("sets visible", sg.setVisible(subject, true));
  assertEq("visible list contains the object",
           sg.getVisibleObjects(altitude).size(), 1);

  assertOk("sets invisible", sg.setVisible(subject, false));
  assertEq("visible list does not contain the object",
           sg.getVisibleObjects(altitude).size(), 0);
}

void setActive() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  sg.insertObject(std::move(obj));

  // The following assertions are order dependent
  assertOk("sets active", sg.setActive(subject, true));
  assert("list of active objects contains object",
         contains(sg.getActiveObjects(), subject));
  assert("list of inactive objects does not contain object",
           !contains(sg.getInactiveObjects(), subject));
  assert("list of solid objects contains object",
         contains(sg.getSolidObjects(), subject));

  assertOk("sets inactive", sg.setActive(subject, false));
  assert("list of active objects does not contain object",
           !contains(sg.getActiveObjects(), subject));
  assert("list of inactive objects contains object",
         contains(sg.getInactiveObjects(), subject));
  assert("list of solid objects does not contain object",
           !contains(sg.getSolidObjects(), subject));
}

auto main() -> int {
  test("insertObject", insertObject);
  test("removeObject", removeObject);
  test("getActiveObjects", getActiveObjects);
  test("getInactiveObjects", getInactiveObjects);
  test("getSolidObjects", getSolidObjects);
  test("getVisibleObjects", getVisibleObjects);
  test("setSolidness", setSolidness);
  test("setAltitude", setAltitude);
  test("setVisible", setVisible);
  test("setActive", setActive);

  return report();
}