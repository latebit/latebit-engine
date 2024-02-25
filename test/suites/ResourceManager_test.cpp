#include "ResourceManager.h"

#include "../lib/test.h"

using namespace lb;
using namespace std;

void ResourceManager_test() {
  test("constructor",
       []() { assert_string("sets type", RM.getType(), "ResourceManager"); });

  test("load and get sprites", []() {
    string label = "test_sprite";
    string filename = "test/fixtures/correct.txt";

    assert_ok("loads sprite successfully", RM.loadSprite(filename, label));
    assert_fail("does not load same label", RM.loadSprite(filename, label));

    for (int i = 1; i < MAX_SPRITES; i++)
      RM.loadSprite(filename, label + to_string(i));

    assert_fail("does not load more than max",
                RM.loadSprite(filename, label + to_string(MAX_SPRITES)));

    assert_fail("does not load missing file",
                RM.loadSprite("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assert_fail("does not load on invalid file",
                RM.loadSprite(anotherFile, label));

    Sprite* sprite = RM.getSprite(label);
    assert("retrieves loaded sprite", sprite != nullptr);
    assert_string("and label is correct", sprite->getLabel(), label);

    sprite = RM.getSprite("another");
    assert("returns null for missing label", sprite == nullptr);

    assert_ok("unloads the sprite", RM.unloadSprite(label));

    sprite = RM.getSprite(label);
    assert("does not retrieve unloaded sprite", sprite == nullptr);
    assert_fail("doesn't unload missing sprite", RM.unloadSprite(label));
  });

  test("shutDown", []() {
    // Warning! Depends on the test above
    RM.shutDown();
    auto sprite = RM.getSprite("test_sprite");
    assert("returns null after shutDown", sprite == nullptr);
  });
}