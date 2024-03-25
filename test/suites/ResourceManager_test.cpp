#include "ResourceManager.h"

#include "../lib/test.h"
#include "AudioManager.h"
#include "Music.h"
#include "Sound.h"

using namespace lb;
using namespace std;

void ResourceManager_test() {
  AM.startUp();
  RM.startUp();

  test("constructor",
       []() { assert_string("sets type", RM.getType(), "ResourceManager"); });

  test("sprites", []() {
    string label = "test_sprite";
    string filename = "test/fixtures/correct.txt";

    assert_ok("loads sprite successfully", RM.loadTextSprite(filename, label));
    assert_fail("does not load same label", RM.loadTextSprite(filename, label));

    for (int i = 1; i < MAX_SPRITES; i++)
      RM.loadTextSprite(filename, label + to_string(i));

    assert_fail("does not load more than max",
                RM.loadTextSprite(filename, label + to_string(MAX_SPRITES)));

    int result = 0;
    for (int i = 1; i < MAX_SPRITES; i++)
      result += RM.unloadSprite(label + to_string(i));
    assert_ok("cleans loaded sprites", result);

    assert_fail("does not load missing file",
                RM.loadTextSprite("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assert_fail("does not load on invalid file",
                RM.loadTextSprite(anotherFile, label));

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

  test("music", []() {
    string label = "test_music";
    string filename = "test/fixtures/sound.wav";

    assert_ok("loads music successfully", RM.loadMusic(filename, label));
    assert_fail("does not load same label", RM.loadMusic(filename, label));

    for (int i = 1; i < MAX_MUSICS; i++)
      RM.loadMusic(filename, label + to_string(i));

    assert_fail("does not load more than max",
                RM.loadMusic(filename, label + to_string(MAX_MUSICS)));

    int result = 0;
    for (int i = 1; i < MAX_MUSICS; i++)
      result += RM.unloadMusic(label + to_string(i));
    assert_ok("cleans loaded musics", result);

    assert_fail("does not load missing file",
                RM.loadMusic("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assert_fail("does not load on invalid file",
                RM.loadMusic(anotherFile, label));

    Music* music = RM.getMusic(label);
    assert("retrieves loaded sprite", music != nullptr);
    assert_string("and label is correct", music->getLabel(), label);

    music = RM.getMusic("another");
    assert("returns null for missing label", music == nullptr);

    assert_ok("unloads the music", RM.unloadMusic(label));

    music = RM.getMusic(label);
    assert("does not retrieve unloaded music", music == nullptr);
    assert_fail("doesn't unload missing music", RM.unloadMusic(label));
  });

  test("sound", []() {
    string label = "test_sound";
    string filename = "test/fixtures/sound.wav";

    assert_ok("loads sound successfully", RM.loadSound(filename, label));
    assert_fail("does not load same label", RM.loadSound(filename, label));

    for (int i = 1; i < MAX_MUSICS; i++)
      RM.loadSound(filename, label + to_string(i));

    assert_fail("does not load more than max",
                RM.loadSound(filename, label + to_string(MAX_MUSICS)));

    int result = 0;
    for (int i = 1; i < MAX_MUSICS; i++)
      result += RM.unloadSound(label + to_string(i));
    assert_ok("cleans loaded sounds", result);

    assert_fail("does not load missing file",
                RM.loadSound("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assert_fail("does not load on invalid file",
                RM.loadSound(anotherFile, label));

    Sound* sound = RM.getSound(label);
    assert("retrieves loaded sprite", sound != nullptr);
    assert_string("and label is correct", sound->getLabel(), label);

    sound = RM.getSound("another");
    assert("returns null for missing label", sound == nullptr);

    assert_ok("unloads the sound", RM.unloadSound(label));

    sound = RM.getSound(label);
    assert("does not retrieve unloaded sound", sound == nullptr);
    assert_fail("doesn't unload missing sound", RM.unloadSound(label));
  });

  test("shutDown", []() {
    // Warning! Depends on the test above
    RM.shutDown();
    auto sprite = RM.getSprite("test_sprite");
    assert("returns null sprite after shutDown", sprite == nullptr);
    auto music = RM.getSprite("test_music");
    assert("returns null music shutDown", music == nullptr);
    auto sound = RM.getSprite("test_sound");
    assert("returns null sound shutDown", sound == nullptr);
  });

  AM.shutDown();
}