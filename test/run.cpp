#include <iomanip>
#include <iostream>

#include "../include/Clock.h"
#include "Animation_test.h"
#include "Box_test.h"
#include "Colors_test.h"
#include "EventCollision_test.h"
#include "EventKeyboard_test.h"
#include "EventOut_test.h"
#include "EventStep_test.h"
#include "Event_test.h"
#include "Frame_test.h"
#include "ObjectListIterator_test.h"
#include "ObjectList_test.h"
#include "Object_test.h"
#include "ResourceManager_test.h"
#include "SpriteParser_test.h"
#include "Sprite_test.h"
#include "Vector_test.h"
#include "WorldManager_test.h"
#include "colors.h"
#include "test.h"
#include "utils_test.h"

void suite(std::string name) { std::cout << yellow("\n" + name + "\n"); }
void timing(float delta) { printf("    Duration: %.2fμs\n", delta); }

auto main() -> int {
  int result = 0;
  df::Clock c;
  c.delta();
  auto initial = c.getNow();

  suite("utils_test");
  result += utils_test();
  timing(c.delta());

  suite("Vector_test");
  result += Vector_test();
  timing(c.delta());

  suite("Object_test");
  result += Object_test();
  timing(c.delta());

  suite("ObjectList_test");
  result += ObjectList_test();
  timing(c.delta());

  suite("ObjectListIterator_test");
  result += ObjectListIterator_test();
  timing(c.delta());

  suite("Event_test");
  result += Event_test();
  timing(c.delta());

  suite("EventStep_test");
  result += EventStep_test();
  timing(c.delta());

  suite("EventKeyboard_test");
  result += EventKeyboard_test();
  timing(c.delta());

  suite("WorldManager_test");
  result += WorldManager_test();
  timing(c.delta());

  suite("EventCollision_test");
  result += EventCollision_test();
  timing(c.delta());

  suite("EventOut_test");
  result += EventOut_test();
  timing(c.delta());

  suite("Frame_test");
  result += Frame_test();
  timing(c.delta());

  suite("Sprite_test");
  result += Sprite_test();
  timing(c.delta());

  suite("Colors_test");
  result += Colors_test();
  timing(c.delta());

  suite("SpriteParser_test");
  result += SpriteParser_test();
  timing(c.delta());

  suite("ResourceManager_test");
  result += ResourceManager_test();
  timing(c.delta());

  suite("Animation_test");
  result += Animation_test();
  timing(c.delta());

  suite("Box_test");
  result += Box_test();
  timing(c.delta());

  auto final = c.getNow();

  if (result == 0) {
    std::cout << std::endl
              << green("Success! All " + getAssertions() + " tests passed. ");
    printf("Total Duration: %.2fms\n",
           static_cast<double>(final - initial) / 1000.0);
  } else {
    std::cout << std::endl
              << red("Failure. " + std::to_string(result) + " failed tests.")
              << std::endl;
  }

  return result;
};
