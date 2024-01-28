#include "SceneGraph.h"

#include "../lib/test.h"
#include "Object.h"

using namespace df;

void SceneGraph_insertObject_test() {
  SceneGraph sg;
  Object obj;
  Object nonSolidObj;
  nonSolidObj.setSolidness(SPECTRAL);

  // Check if the object was inserted successfully
  assert_ok("inserts object into the scene graph", sg.insertObject(&obj));
  assert_int("scene graph contains the object",
             sg.getActiveObjects().getCount(), 1);
  assert_int("scene graph contains the solid object",
             sg.getSolidObjects().getCount(), 1);
  assert_int("scene graph contains the visible object",
             sg.getVisibleObjects(obj.getAltitude()).getCount(), 1);
  assert_ok("inserts non solid object into the scene graph",
            sg.insertObject(&nonSolidObj));
  assert_int("scene graph does not contain the non-solid object",
             sg.getSolidObjects().getCount(), 1);
}

void SceneGraph_removeObject_test() {
  SceneGraph sg;
  Object obj;

  // Insert object into the scene graph
  sg.insertObject(&obj);

  // Check if the object was removed successfully
  assert_ok("removes object from the scene graph", sg.removeObject(&obj));
  assert_int("scene graph does not contain the object",
             sg.getActiveObjects().getCount(), 0);
  assert_int("scene graph does not contain the solid object",
             sg.getSolidObjects().getCount(), 0);
  assert_int("scene graph does not contain the visible object",
             sg.getVisibleObjects(obj.getAltitude()).getCount(), 0);
}

void SceneGraph_getActiveObjects_test() {
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
  assert_int("returns all objects from the scene graph", objects.getCount(), 3);
  assert("contains object 1", objects.find(&obj1) > -1);
  assert("does not contain object 2", objects.find(&obj2) == -1);
  assert("contains object 3", objects.find(&obj3) > -1);
  assert("contains object 4", objects.find(&obj4) > -1);
}

void SceneGraph_getInactiveObjects_test() {
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
  assert_int("returns inactive objects from the scene graph",
             inactiveObjects.getCount(), 1);
  assert_int("does not contain object 1", inactiveObjects.find(&obj1), -1);
  assert("contains object 2", inactiveObjects.find(&obj2) > -1);
  assert_int("does not contain object 3", inactiveObjects.find(&obj3), -1);
  assert_int("does not contain object 4", inactiveObjects.find(&obj4), -1);
}

void SceneGraph_getSolidObjects_test() {
  SceneGraph sg;
  Object obj1, obj2, obj3;
  obj2.setSolidness(SPECTRAL);

  // Insert objects into the scene graph
  sg.insertObject(&obj1);
  sg.insertObject(&obj2);
  sg.insertObject(&obj3);

  // Get solid objects from the scene graph
  ObjectList solidObjects = sg.getSolidObjects();

  // Check if solid objects are returned
  assert_int("returns solid objects from the scene graph",
             solidObjects.getCount(), 2);
  assert("contains solid object 1", solidObjects.find(&obj1) > -1);
  assert("does not contain solid object 2", solidObjects.find(&obj2) == -1);
  assert("contains solid object 3", solidObjects.find(&obj3) > -1);
}

void SceneGraph_getVisibleObjects_test() {
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
  assert_int("returns visible objects with altitude 1 from the scene graph",
             visibleObjects.getCount(), 2);
  assert("contains visible object 2", visibleObjects.find(&obj2) > -1);
  assert("contains visible object 3", visibleObjects.find(&obj3) > -1);
}

void SceneGraph_setSolidness_test() {
  SceneGraph sg;
  Object obj;

  // Insert object into the scene graph
  sg.insertObject(&obj);

  // Set solidness of the object to HARD
  int result = sg.setSolidness(&obj, HARD);

  // Check if the solidness was set successfully
  assert_int("sets solidness of the object to HARD", result, 0);
  assert_int("scene graph contains the solid object",
             sg.getSolidObjects().getCount(), 1);

  // Set solidness of the object to SPECTRAL
  result = sg.setSolidness(&obj, SPECTRAL);

  // Check if the solidness was set successfully
  assert_int("sets solidness of the object to SPECTRAL", result, 0);
  assert_int("scene graph does not contain the solid object",
             sg.getSolidObjects().getCount(), 0);
}

void SceneGraph_setAltitude_test() {
  SceneGraph sg;
  Object obj;

  // Insert object into the scene graph
  sg.insertObject(&obj);

  // Set altitude of the object to 1
  int result = sg.setAltitude(&obj, 1);

  // Check if the altitude was set successfully
  assert_int("sets altitude of the object to 1", result, 0);
  assert_int("scene graph contains the visible object",
             sg.getVisibleObjects(1).getCount(), 1);

  // Set altitude of the object to -1 (invalid)
  result = sg.setAltitude(&obj, -1);

  // Check if the altitude was set successfully
  assert_int("does not set altitude of the object to -1", result, -1);
  assert_int("scene graph does not contain the visible object",
             sg.getVisibleObjects(-1).getCount(), 0);
}

void SceneGraph_setVisible_test() {
  SceneGraph sg;
  Object obj;

  sg.insertObject(&obj);

  // The following assertions are order dependent

  assert_ok("sets visible", sg.setVisible(&obj, true));
  assert_int("visible list contains the object",
             sg.getVisibleObjects(obj.getAltitude()).getCount(), 1);

  assert_ok("sets invisible", sg.setVisible(&obj, false));
  assert_int("visible list does not contain the object",
             sg.getVisibleObjects(obj.getAltitude()).getCount(), 0);
}

void SceneGraph_setActive_test() {
  SceneGraph sg;
  Object obj;

  sg.insertObject(&obj);

  // The following assertions are order dependent

  assert_ok("sets active", sg.setActive(&obj, true));
  assert("list of active objects contains object",
         sg.getActiveObjects().find(&obj) > -1);
  assert_int("list of inactive objects does not contain object",
             sg.getInactiveObjects().find(&obj), -1);
  assert("list of solid objects contains object",
         sg.getSolidObjects().find(&obj) > -1);

  assert_ok("sets inactive", sg.setActive(&obj, false));
  assert_int("list of active objects does not contain object",
             sg.getActiveObjects().find(&obj), -1);
  assert("list of inactive objects contains object",
         sg.getInactiveObjects().find(&obj) > -1);
  assert_int("list of solid objects does not contain object",
             sg.getSolidObjects().find(&obj), -1);
}

void SceneGraph_test() {
  test("insertObject", SceneGraph_insertObject_test);
  test("removeObject", SceneGraph_removeObject_test);
  test("getActiveObjects", SceneGraph_getActiveObjects_test);
  test("getInactiveObjects", SceneGraph_getInactiveObjects_test);
  test("getSolidObjects", SceneGraph_getSolidObjects_test);
  test("getVisibleObjects", SceneGraph_getVisibleObjects_test);
  test("setSolidness", SceneGraph_setSolidness_test);
  test("setAltitude", SceneGraph_setAltitude_test);
  test("setVisible", SceneGraph_setVisible_test);
  test("setActive", SceneGraph_setActive_test);
}