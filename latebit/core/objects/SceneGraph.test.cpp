#include "latebit/core/objects/SceneGraph.h"

#include "latebit/core/objects/Object.h"
#include "test/lib/test.h"

using namespace lb;

void insertObject() {
  SceneGraph sg;
  Object obj;
  Object nonSolidObj;
  nonSolidObj.setSolidness(Solidness::SPECTRAL);

  // Check if the object was inserted successfully
  assertOk("inserts object into the scene graph", sg.insertObject(&obj));
  assertEq("scene graph contains the object", sg.getActiveObjects().getCount(),
           1);
  assertEq("scene graph contains the solid object",
           sg.getSolidObjects().getCount(), 1);
  assertEq("scene graph contains the visible object",
           sg.getVisibleObjects(obj.getAltitude()).getCount(), 1);
  assertOk("inserts non solid object into the scene graph",
           sg.insertObject(&nonSolidObj));
  assertEq("scene graph does not contain the non-solid object",
           sg.getSolidObjects().getCount(), 1);
}

void removeObject() {
  SceneGraph sg;
  Object obj;

  // Insert object into the scene graph
  sg.insertObject(&obj);

  // Check if the object was removed successfully
  assertOk("removes object from the scene graph", sg.removeObject(&obj));
  assertEq("scene graph does not contain the object",
           sg.getActiveObjects().getCount(), 0);
  assertEq("scene graph does not contain the solid object",
           sg.getSolidObjects().getCount(), 0);
  assertEq("scene graph does not contain the visible object",
           sg.getVisibleObjects(obj.getAltitude()).getCount(), 0);
}

void getActiveObjects() {
  SceneGraph sg;
  Object obj1, obj2, obj3, obj4;
  obj2.setActive(false);

  // Insert objects into the scene graph
  sg.insertObject(&obj1);
  sg.insertObject(&obj2);
  sg.insertObject(&obj3);
  sg.insertObject(&obj4);

  // Get all objects from the scene graph
  ObjectList objects = sg.getActiveObjects();

  // Check if all objects are returned
  assertEq("returns all objects from the scene graph", objects.getCount(), 3);
  assert("contains object 1", objects.find(&obj1) > -1);
  assert("does not contain object 2", objects.find(&obj2) == -1);
  assert("contains object 3", objects.find(&obj3) > -1);
  assert("contains object 4", objects.find(&obj4) > -1);
}

void getInactiveObjects() {
  SceneGraph sg;
  Object obj1, obj2, obj3, obj4;
  obj2.setActive(false);

  // Insert objects into the scene graph
  sg.insertObject(&obj1);
  sg.insertObject(&obj2);
  sg.insertObject(&obj3);
  sg.insertObject(&obj4);

  // Get inactive objects from the scene graph
  ObjectList inactiveObjects = sg.getInactiveObjects();

  // Check if inactive objects are returned
  assertEq("returns inactive objects from the scene graph",
           inactiveObjects.getCount(), 1);
  assertEq("does not contain object 1", inactiveObjects.find(&obj1), -1);
  assert("contains object 2", inactiveObjects.find(&obj2) > -1);
  assertEq("does not contain object 3", inactiveObjects.find(&obj3), -1);
  assertEq("does not contain object 4", inactiveObjects.find(&obj4), -1);
}

void getSolidObjects() {
  SceneGraph sg;
  Object obj1, obj2, obj3;
  obj2.setSolidness(Solidness::SPECTRAL);

  // Insert objects into the scene graph
  sg.insertObject(&obj1);
  sg.insertObject(&obj2);
  sg.insertObject(&obj3);

  // Get solid objects from the scene graph
  ObjectList solidObjects = sg.getSolidObjects();

  // Check if solid objects are returned
  assertEq("returns solid objects from the scene graph",
           solidObjects.getCount(), 2);
  assert("contains solid object 1", solidObjects.find(&obj1) > -1);
  assert("does not contain solid object 2", solidObjects.find(&obj2) == -1);
  assert("contains solid object 3", solidObjects.find(&obj3) > -1);
}

void getVisibleObjects() {
  SceneGraph sg;
  Object obj1, obj2, obj3;

  // Set altitudes for the objects
  obj1.setAltitude(0);
  obj2.setAltitude(1);
  obj3.setAltitude(1);

  // Insert objects into the scene graph
  sg.insertObject(&obj1);
  sg.insertObject(&obj2);
  sg.insertObject(&obj3);

  // Get visible objects with altitude 1 from the scene graph
  ObjectList visibleObjects = sg.getVisibleObjects(1);

  // Check if visible objects with altitude 1 are returned
  assertEq("returns visible objects with altitude 1 from the scene graph",
           visibleObjects.getCount(), 2);
  assert("contains visible object 2", visibleObjects.find(&obj2) > -1);
  assert("contains visible object 3", visibleObjects.find(&obj3) > -1);
}

void setSolidness() {
  SceneGraph sg;
  Object obj;

  // Insert object into the scene graph
  sg.insertObject(&obj);

  // Set solidness of the object to HARD
  int result = sg.setSolidness(&obj, Solidness::HARD);

  // Check if the solidness was set successfully
  assertEq("sets solidness of the object to HARD", result, 0);
  assertEq("scene graph contains the solid object",
           sg.getSolidObjects().getCount(), 1);

  // Set solidness of the object to SPECTRAL
  result = sg.setSolidness(&obj, Solidness::SPECTRAL);

  // Check if the solidness was set successfully
  assertEq("sets solidness of the object to SPECTRAL", result, 0);
  assertEq("scene graph does not contain the solid object",
           sg.getSolidObjects().getCount(), 0);
}

void setAltitude() {
  SceneGraph sg;
  Object obj;

  // Insert object into the scene graph
  sg.insertObject(&obj);

  // Set altitude of the object to 1
  int result = sg.setAltitude(&obj, 1);

  // Check if the altitude was set successfully
  assertEq("sets altitude of the object to 1", result, 0);
  assertEq("scene graph contains the visible object",
           sg.getVisibleObjects(1).getCount(), 1);

  // Set altitude of the object to -1 (invalid)
  result = sg.setAltitude(&obj, -1);

  // Check if the altitude was set successfully
  assertEq("does not set altitude of the object to -1", result, -1);
  assertEq("scene graph does not contain the visible object",
           sg.getVisibleObjects(-1).getCount(), 0);
}

void setVisible() {
  SceneGraph sg;
  Object obj;

  sg.insertObject(&obj);

  // The following assertions are order dependent

  assertOk("sets visible", sg.setVisible(&obj, true));
  assertEq("visible list contains the object",
           sg.getVisibleObjects(obj.getAltitude()).getCount(), 1);

  assertOk("sets invisible", sg.setVisible(&obj, false));
  assertEq("visible list does not contain the object",
           sg.getVisibleObjects(obj.getAltitude()).getCount(), 0);
}

void setActive() {
  SceneGraph sg;
  Object obj;

  sg.insertObject(&obj);

  // The following assertions are order dependent

  assertOk("sets active", sg.setActive(&obj, true));
  assert("list of active objects contains object",
         sg.getActiveObjects().find(&obj) > -1);
  assertEq("list of inactive objects does not contain object",
           sg.getInactiveObjects().find(&obj), -1);
  assert("list of solid objects contains object",
         sg.getSolidObjects().find(&obj) > -1);

  assertOk("sets inactive", sg.setActive(&obj, false));
  assertEq("list of active objects does not contain object",
           sg.getActiveObjects().find(&obj), -1);
  assert("list of inactive objects contains object",
         sg.getInactiveObjects().find(&obj) > -1);
  assertEq("list of solid objects does not contain object",
           sg.getSolidObjects().find(&obj), -1);
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