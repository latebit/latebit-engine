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
  assert_int("scene graph contains the object", sg.getAllObjects().getCount(),
             1);
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
             sg.getAllObjects().getCount(), 0);
  assert_int("scene graph does not contain the solid object",
             sg.getSolidObjects().getCount(), 0);
  assert_int("scene graph does not contain the visible object",
             sg.getVisibleObjects(obj.getAltitude()).getCount(), 0);
}

void SceneGraph_getAllObjects_test() {
  SceneGraph sg;
  Object obj1, obj2, obj3;

  // Insert objects into the scene graph
  sg.insertObject(&obj1);
  sg.insertObject(&obj2);
  sg.insertObject(&obj3);

  // Get all objects from the scene graph
  ObjectList objects = sg.getAllObjects();

  // Check if all objects are returned
  assert_int("returns all objects from the scene graph", objects.getCount(), 3);
  assert("contains object 1", objects.find(&obj1) > -1);
  assert("contains object 2", objects.find(&obj2) > -1);
  assert("contains object 3", objects.find(&obj3) > -1);
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

void SceneGraph_test() {
  test("insertObject", SceneGraph_insertObject_test);
  test("removeObject", SceneGraph_removeObject_test);
  test("getAllObjects", SceneGraph_getAllObjects_test);
  test("getSolidObjects", SceneGraph_getSolidObjects_test);
  test("getVisibleObjects", SceneGraph_getVisibleObjects_test);
  test("setSolidness", SceneGraph_setSolidness_test);
  test("setAltitude", SceneGraph_setAltitude_test);
}