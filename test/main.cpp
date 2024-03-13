#include "Logger.h"
#include "lib/run.h"
#include "lib/test.h"
#include "suites/Animation_test.cpp"
#include "suites/Box_test.cpp"
#include "suites/Colors_test.cpp"
#include "suites/Configuration_test.cpp"
#include "suites/EventCollision_test.cpp"
#include "suites/EventKeyboard_test.cpp"
#include "suites/EventOut_test.cpp"
#include "suites/EventStep_test.cpp"
#include "suites/Event_test.cpp"
#include "suites/Frame_test.cpp"
#include "suites/GameManager_test.cpp"
#include "suites/Manager_test.cpp"
#include "suites/Music_test.cpp"
#include "suites/ObjectListIterator_test.cpp"
#include "suites/ObjectList_test.cpp"
#include "suites/Object_test.cpp"
#include "suites/ResourceManager_test.cpp"
#include "suites/SceneGraph_test.cpp"
#include "suites/Sound_test.cpp"
#include "suites/SpriteParser_test.cpp"
#include "suites/Sprite_test.cpp"
#include "suites/Vector_test.cpp"
#include "suites/WorldManager_test.cpp"
#include "suites/utils_test.cpp"

auto main() -> int {
  Log.setLevel(lb::DEBUG);
  return run([]() -> void {
    suite("Animation", Animation_test);
    suite("Box", Box_test);
    suite("Colors", Colors_test);
    suite("Configuration", Configuration_test);
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
    suite("SceneGraph", SceneGraph_test);
    suite("Sound", Sound_test);
    suite("Sprite", Sprite_test);
    suite("SpriteParser", SpriteParser_test);
    suite("utils", utils_test);
    suite("Vector", Vector_test);
    suite("WorldManager", WorldManager_test);
  });
};
