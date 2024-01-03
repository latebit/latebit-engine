#include <iomanip>
#include <iostream>

#include "EventKeyboard_test.h"
#include "EventStep_test.h"
#include "Event_test.h"
#include "ObjectListIterator_test.h"
#include "ObjectList_test.h"
#include "Object_test.h"
#include "Vector_test.h"
#include "WorldManager_test.h"
#include "colors.h"
#include "test.h"
#include "utils_test.h"

void suite(std::string name) { std::cout << yellow("\n" + name + "\n"); }

int main() {
  int result = 0;
  suite("utils_test");
  result += utils_test();

  suite("Vector_test");
  result += Vector_test();

  suite("Object_test");
  result += Object_test();

  suite("ObjectList_test");
  result += ObjectList_test();

  suite("ObjectListIterator_test");
  result += ObjectListIterator_test();

  suite("Event_test");
  result += Event_test();

  suite("EventStep_test");
  result += EventStep_test();

  suite("EventKeyboard_test");
  result += EventKeyboard_test();

  suite("WorldManager_test");
  result += WorldManager_test();

  if (result == 0) {
    std::cout << std::endl
              << green("Success! All " + getAssertions() + " tests passed.")
              << std::endl;
  } else {
    std::cout << std::endl
              << green("Failure. " + std::to_string(result) + " failed tests.")
              << std::endl;
  }

  return result;
};
