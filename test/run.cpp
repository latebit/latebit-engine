#include <iostream>

#include "EventStep_test.h"
#include "Event_test.h"
#include "ObjectListIterator_test.h"
#include "ObjectList_test.h"
#include "Object_test.h"
#include "Vector_test.h"
#include "test.h"
#include "utils_test.h"

void suite(std::string name) { std::cout << "\n[[[ " + name + " ]]]\n\n"; }

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

  if (result == 0) {
    std::cout << "Success! All " << getAssertions() << " tests passed."
              << std::endl;
  } else {
    printf("Failure. %d failed tests.\n", result);
  }

  return result;
};
