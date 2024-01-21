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
#include "GameManager_test.h"
#include "Manager_test.h"
#include "Music_test.h"
#include "ObjectListIterator_test.h"
#include "ObjectList_test.h"
#include "Object_test.h"
#include "ResourceManager_test.h"
#include "Sound_test.h"
#include "SpriteParser_test.h"
#include "Sprite_test.h"
#include "Vector_test.h"
#include "WorldManager_test.h"
#include "colors.h"
#include "test.h"
#include "utils_test.h"

auto main() -> int {
  auto c = getClock();
  auto initial = c.getNow();

  suite("Animation", Animation_test);
  suite("Box", Box_test);
  suite("Colors", Colors_test);
  suite("Event", Event_test);
  suite("EventCollision", EventCollision_test);
  suite("EventKeyboard", EventKeyboard_test);
  suite("EventOut", EventOut_test);
  suite("EventStep", EventStep_test);
  suite("Frame", Frame_test);
  suite("GameManager", GameManager_test);
  suite("Manager", Manager_test);
  suite("Music", Music_test);
  suite("Object", Object_test);
  suite("ObjectList", ObjectList_test);
  suite("ObjectListIterator", ObjectListIterator_test);
  suite("ResourceManager", ResourceManager_test);
  suite("Sound", Sound_test);
  suite("Sprite", Sprite_test);
  suite("SpriteParser", SpriteParser_test);
  suite("utils", utils_test);
  suite("Vector", Vector_test);
  int result = suite("WorldManager", WorldManager_test);

  auto final = c.getNow();

  if (result == 0) {
    std::cout << std::endl
              << green("Success! All " + getAssertions() +
                       " assertions passed. ");
    printf("Total Duration: %.2fms\n",
           static_cast<double>(final - initial) / 1000.0);
  } else {
    std::cout << std::endl
              << red("Failure. " + std::to_string(result) + " failed tests.")
              << std::endl;
  }

  return result;
};
