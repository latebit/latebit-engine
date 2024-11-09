#include "SceneGraph.h"

#include <memory>

#include "latebit/core/world/Object.h"
#include "latebit/core/world/ObjectUtils.h"
#include "test/lib/test.h"

using namespace lb;

void insertObject() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto altitude = obj->getAltitude();
  obj->setBodyType(BodyType::DYNAMIC);
  auto nonSolidObj = make_unique<Object>();
  nonSolidObj->setBodyType(BodyType::KINEMATIC);

  // Check if the object was inserted successfully
  assertOk("inserts object into the scene graph", sg.insertObject(obj.get()));
  assertEq("scene graph contains the object", sg.getActiveObjects().size(), 1);
  assertEq("scene graph contains the active object",
           sg.getActiveObjects().size(), 1);
  assertEq("scene graph contains the solid object", sg.getSolidObjects().size(),
           1);
  assertEq("scene graph contains the visible object",
           sg.getVisibleObjects(altitude).size(), 1);
  assertOk("inserts non solid object into the scene graph",
           sg.insertObject(nonSolidObj.get()));
  assertEq("scene graph does not contain the non-solid object",
           sg.getSolidObjects().size(), 1);
}

void removeObject() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  auto altitude = obj->getAltitude();

  // Insert object into the scene graph
  sg.insertObject(obj.get());

  // Check if the object was removed successfully
  assertOk("removes object from the scene graph", sg.removeObject(subject));
  assertEq("scene graph does not contain the object",
           sg.getActiveObjects().size(), 0);
  assertEq("scene graph does not contain the active object",
           sg.getActiveObjects().size(), 0);
  assertEq("scene graph does not contain the solid object",
           sg.getSolidObjects().size(), 0);
  assertEq("scene graph does not contain the visible object",
           sg.getVisibleObjects(altitude).size(), 0);
}

void getActiveObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(),
       obj3 = make_unique<Object>(), obj4 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get(),
       subject4 = obj4.get();
  obj2->setActive(false);

  // Insert objects into the scene graph
  sg.insertObject(obj1.get());
  sg.insertObject(obj2.get());
  sg.insertObject(obj3.get());
  sg.insertObject(obj4.get());

  // Check if all objects are returned
  assertEq("returns all objects from the scene graph",
           sg.getActiveObjects().size(), 3);
  assert("contains object 1", contains(sg.getActiveObjects(), subject1));
  assert("does not contain object 2",
         !contains(sg.getActiveObjects(), subject2));
  assert("contains object 3", contains(sg.getActiveObjects(), subject3));
  assert("contains object 4", contains(sg.getActiveObjects(), subject4));
}

void getInactiveObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(),
       obj3 = make_unique<Object>(), obj4 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get(),
       subject4 = obj4.get();
  obj2->setActive(false);

  sg.insertObject(obj1.get());
  sg.insertObject(obj2.get());
  sg.insertObject(obj3.get());
  sg.insertObject(obj4.get());

  auto inactive = sg.getInactiveObjects();

  assertEq("returns inactive objects from the scene graph", inactive.size(), 1);
  assert("does not contain object 1", !contains(inactive, subject1));
  assert("contains object 2", contains(inactive, subject2));
  assert("does not contain object 3", !contains(inactive, subject3));
  assert("does not contain object 4", !contains(inactive, subject4));
}

void getSolidObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(),
       obj3 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get();
  obj1->setBodyType(BodyType::DYNAMIC);
  obj2->setBodyType(BodyType::KINEMATIC);
  obj3->setBodyType(BodyType::DYNAMIC);

  sg.insertObject(obj1.get());
  sg.insertObject(obj2.get());
  sg.insertObject(obj3.get());

  auto solid = sg.getSolidObjects();

  assertEq("returns solid objects from the scene graph", solid.size(), 2);
  assert("contains solid object 1", contains(solid, subject1));
  assert("does not contain solid object 2", !contains(solid, subject2));
  assert("contains solid object 3", contains(solid, subject3));
}

void getVisibleObjects() {
  SceneGraph sg;
  auto obj1 = make_unique<Object>(), obj2 = make_unique<Object>(),
       obj3 = make_unique<Object>();
  auto subject1 = obj1.get(), subject2 = obj2.get(), subject3 = obj3.get();

  // Set altitudes for the objects
  obj1->setAltitude(0);
  obj2->setAltitude(1);
  obj3->setAltitude(1);

  // Insert objects into the scene graph
  sg.insertObject(obj1.get());
  sg.insertObject(obj2.get());
  sg.insertObject(obj3.get());

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

  sg.removeObject(subject1);
  sg.removeObject(subject2);
  visible = sg.getVisibleObjects(0);
  assert("does not contain visible object 1", !contains(visible, subject1));
  assertEq("size", visible.size(), 0);
  visible = sg.getVisibleObjects(1);
  assert("does not contain visible object 2", !contains(visible, subject2));
  assertEq("size", visible.size(), 1);
}

void setBodyType() {
  SceneGraph sg;
  auto dynamic = make_unique<Object>();
  sg.insertObject(dynamic.get());

  sg.setBodyType(dynamic.get(), BodyType::DYNAMIC);
  assertEq("graph contains the object", sg.getSolidObjects().size(), 1);

  auto kinematic = make_unique<Object>();
  sg.setBodyType(kinematic.get(), BodyType::KINEMATIC);
  assertEq("graph does not contain the object", sg.getSolidObjects().size(), 1);
}

void setAltitude() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  sg.insertObject(subject);

  sg.setAltitude(subject, 1);
  assertEq("scene graph contains the visible object",
           sg.getVisibleObjects(1).size(), 1);

  sg.setAltitude(subject, -1);
  assertEq("scene graph does not contain the visible object",
           sg.getVisibleObjects(-1).size(), 0);
}

void setVisible() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  auto altitude = obj->getAltitude();
  sg.insertObject(subject);

  // The following assertions are order dependent
  sg.setVisible(subject, true);
  assertEq("visible list contains the object",
           sg.getVisibleObjects(altitude).size(), 1);

  sg.setVisible(subject, false);
  assertEq("visible list does not contain the object",
           sg.getVisibleObjects(altitude).size(), 0);
}

void setActive() {
  SceneGraph sg;
  auto obj = make_unique<Object>();
  auto subject = obj.get();
  sg.insertObject(subject);

  // The following assertions are order dependent
  sg.setActive(subject, true);
  assert("list of active objects contains object",
         contains(sg.getActiveObjects(), subject));
  assert("list of inactive objects does not contain object",
         !contains(sg.getInactiveObjects(), subject));

  sg.setActive(subject, false);
  assert("list of active objects does not contain object",
         !contains(sg.getActiveObjects(), subject));
  assert("list of inactive objects contains object",
         contains(sg.getInactiveObjects(), subject));
}

auto main() -> int {
  test("insertObject", insertObject);
  test("removeObject", removeObject);
  test("getActiveObjects", getActiveObjects);
  test("getInactiveObjects", getInactiveObjects);
  test("getSolidObjects", getSolidObjects);
  test("getVisibleObjects", getVisibleObjects);
  test("setBodyType", setBodyType);
  test("setAltitude", setAltitude);
  test("setVisible", setVisible);
  test("setActive", setActive);

  return report();
}