#include "core/ResourceManager.h"

#include "../lib/test.h"
#include "core/audio/AudioManager.h"
#include "core/audio/Music.h"
#include "core/audio/Sound.h"

using namespace lb;
using namespace std;

void ResourceManager_test() {
  AM.startUp();
  RM.startUp();

  test("constructor",
       []() { assertEq("sets type", RM.getType(), "ResourceManager"); });

  test("sprites", []() {
    string label = "test_sprite";
    string filename = "test/fixtures/correct.txt";

    assertOk("loads sprite successfully", RM.loadTextSprite(filename, label));
    assertFail("does not load same label", RM.loadTextSprite(filename, label));

    for (int i = 1; i < MAX_SPRITES; i++)
      RM.loadTextSprite(filename, label + to_string(i));

    assertFail("does not load more than max",
               RM.loadTextSprite(filename, label + to_string(MAX_SPRITES)));

    int result = 0;
    for (int i = 1; i < MAX_SPRITES; i++)
      result += RM.unloadSprite(label + to_string(i));
    assertOk("cleans loaded sprites", result);

    assertFail("does not load missing file",
               RM.loadTextSprite("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assertFail("does not load on invalid file",
               RM.loadTextSprite(anotherFile, label));

    Sprite* sprite = RM.getSprite(label);
    assert("retrieves loaded sprite", sprite != nullptr);
    assertEq("and label is correct", sprite->getLabel(), label);

    sprite = RM.getSprite("another");
    assert("returns null for missing label", sprite == nullptr);

    assertOk("unloads the sprite", RM.unloadSprite(label));

    sprite = RM.getSprite(label);
    assert("does not retrieve unloaded sprite", sprite == nullptr);
    assertFail("doesn't unload missing sprite", RM.unloadSprite(label));
  });

  test("music", []() {
    string label = "test_music";
    string filename = "test/fixtures/sound.wav";

    assertOk("loads music successfully", RM.loadMusic(filename, label));
    assertFail("does not load same label", RM.loadMusic(filename, label));

    for (int i = 1; i < MAX_MUSICS; i++)
      RM.loadMusic(filename, label + to_string(i));

    assertFail("does not load more than max",
               RM.loadMusic(filename, label + to_string(MAX_MUSICS)));

    int result = 0;
    for (int i = 1; i < MAX_MUSICS; i++)
      result += RM.unloadMusic(label + to_string(i));
    assertOk("cleans loaded musics", result);

    assertFail("does not load missing file",
               RM.loadMusic("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assertFail("does not load on invalid file",
               RM.loadMusic(anotherFile, label));

    Music* music = RM.getMusic(label);
    assert("retrieves loaded sprite", music != nullptr);
    assertEq("and label is correct", music->getLabel(), label);

    music = RM.getMusic("another");
    assert("returns null for missing label", music == nullptr);

    assertOk("unloads the music", RM.unloadMusic(label));

    music = RM.getMusic(label);
    assert("does not retrieve unloaded music", music == nullptr);
    assertFail("doesn't unload missing music", RM.unloadMusic(label));
  });

  test("sound", []() {
    string label = "test_sound";
    string filename = "test/fixtures/sound.wav";

    assertOk("loads sound successfully", RM.loadSound(filename, label));
    assertFail("does not load same label", RM.loadSound(filename, label));

    for (int i = 1; i < MAX_MUSICS; i++)
      RM.loadSound(filename, label + to_string(i));

    assertFail("does not load more than max",
               RM.loadSound(filename, label + to_string(MAX_MUSICS)));

    int result = 0;
    for (int i = 1; i < MAX_MUSICS; i++)
      result += RM.unloadSound(label + to_string(i));
    assertOk("cleans loaded sounds", result);

    assertFail("does not load missing file",
               RM.loadSound("does-not-exist", label));

    string anotherFile = "test/fixtures/missing-frame.txt";
    assertFail("does not load on invalid file",
               RM.loadSound(anotherFile, label));

    Sound* sound = RM.getSound(label);
    assert("retrieves loaded sprite", sound != nullptr);
    assertEq("and label is correct", sound->getLabel(), label);

    sound = RM.getSound("another");
    assert("returns null for missing label", sound == nullptr);

    assertOk("unloads the sound", RM.unloadSound(label));

    sound = RM.getSound(label);
    assert("does not retrieve unloaded sound", sound == nullptr);
    assertFail("doesn't unload missing sound", RM.unloadSound(label));
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