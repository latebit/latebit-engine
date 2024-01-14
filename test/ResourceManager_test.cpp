#include "../include/ResourceManager.h"

#include "SpriteParser_test.h"
#include "test.h"

auto ResourceManager_test() -> int {
  int result = 0;

  result += assert_string("sets type", RM.getType(), "ResourceManager");

  std::string filename = "sprite.txt";
  std::string label = "test_sprite";

  makeFile(filename, "1\n3\n4\n2\nblue\n***\n***\n***\n***");
  result +=
    assert_ok("loads sprite successfully", RM.loadSprite(filename, label));

  result +=
    assert_fail("does not load same label", RM.loadSprite(filename, label));

  for (int i = 1; i < df::MAX_SPRITES; i++)
    RM.loadSprite(filename, label + std::to_string(i));

  result += assert_fail(
    "does not load more than max",
    RM.loadSprite(filename, label + std::to_string(df::MAX_SPRITES)));
  remove(filename.c_str());

  result +=
    assert_fail("does not load missing file", RM.loadSprite(filename, label));

  std::string anotherFile = "another_sprite.txt";
  makeFile(anotherFile, "2\n3\n4\n2\nblue\n***\n***\n***\n***");
  result += assert_fail("does not load on invalid file",
                        RM.loadSprite(anotherFile, label));
  remove(anotherFile.c_str());

  df::Sprite* sprite = RM.getSprite(label);
  result += assert("retrieves loaded sprite", sprite != nullptr);
  if (sprite != nullptr) {
    result += assert_string("and label is correct", sprite->getLabel(), label);
  }

  sprite = RM.getSprite("another");
  result += assert("returns null for missing label", sprite == nullptr);

  result += assert_ok("unloads the sprite", RM.unloadSprite(label));

  sprite = RM.getSprite(label);
  result += assert("does not retrieve unloaded sprite", sprite == nullptr);

  result +=
    assert_fail("doesn't unload missing sprite", RM.unloadSprite(label));

  RM.shutDown();
  sprite = RM.getSprite(label);
  result += assert("returns null after shutDown", sprite == nullptr);

  return result;
}