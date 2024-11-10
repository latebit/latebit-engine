#include "latebit/core/GameManager.h"

#include <thread>

#include "latebit/core/audio/AudioManager.h"
#include "latebit/core/configuration/Configuration.h"
#include "latebit/core/events/EventStep.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/utils/Logger.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace lb {

// Sleep for a given number of microseconds
void sleep(long int microseconds) {
  this_thread::sleep_for(chrono::microseconds(microseconds));
}

GameManager::GameManager() : Manager("GameManager") {
  Log.debug("GameManager::GameManager(): Created GameManager");
}

auto GameManager::startUp() -> int {
  if (WM.startUp() != 0) {
    Log.error("GameManager::startUp(): Error starting WorldManager");
    return -1;
  }

  if (DM.startUp() != 0) {
    Log.error("GameManager::startUp(): Error starting DisplayManager");
    return -1;
  }

  if (IM.startUp() != 0) {
    Log.error("GameManager::startUp(): Error starting InputManager");
    return -1;
  }

  if (AM.startUp() != 0) {
    Log.error("GameManager::startUp(): Error starting AudioManager");
    return -1;
  }

  this->setRandomSeed();
  this->setFrameTime(1000 / Configuration::getMaxFrameRate());

  // By default boundary equates view and it's the whole window
  Box boundary(Vector(0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);
  WM.setBoundary(boundary);
  WM.getCamera().setView(boundary);

  Log.info("GameManager::startUp(): Started successfully");
  return Manager::startUp();
}

auto GameManager::getInstance() -> GameManager& {
  static GameManager instance;
  return instance;
}

void GameManager::shutDown() {
  if (!this->isStarted()) {
    Log.warning("GameManager::shutDown(): GameManager already shut down");
    return;
  }

  setGameOver(true);
  AM.shutDown();
  IM.shutDown();
  DM.shutDown();
  WM.shutDown();
  Manager::shutDown();
  Log.info("GameManager::shutDown(): Shut down successfully");
}

auto GameManager::isValid(string eventType) const -> bool {
  return eventType == STEP_EVENT;
}

#ifndef __EMSCRIPTEN__
void GameManager::run() {
  if (!this->isStarted()) {
    Log.error(
      "GameManager::run(): GameManager not started. Please call `startUp` "
      "before running the game");
    return;
  }

  long int loopTime = 0;
  long int steps = 0;

  while (!gameOver) {
    clock->delta();

    // Send a step event to all subscribers
    const auto evt = EventStep(steps++);
    broadcast(&evt);

    IM.getInput();

    if (!paused) {
      WM.update();
    }

    WM.draw();
    DM.swapBuffers();

    loopTime = clock->delta();
    sleep(frameTime - loopTime);
  }
}

void GameManager::setGameOver(bool gameOver) { this->gameOver = gameOver; }

void GameManager::pause() { this->paused = true; }

void GameManager::resume() { this->paused = false; }

auto GameManager::isPaused() const -> bool { return this->paused; }

#endif

#ifdef __EMSCRIPTEN__
struct EmscriptenLoopArgs {
  long int* adjustTime = 0;
  long int* loopTime = 0;
  long int* steps = 0;
  Clock* clock = 0;
  int frameTime = 0;
  bool* paused = 0;
};

void GameManager::loop(void* a) {
  EmscriptenLoopArgs* args = (EmscriptenLoopArgs*)a;
  // Send a step event to all subscribers
  const auto evt = EventStep(++(*args->steps));
  GM.broadcast(&evt);

  IM.getInput();
  if (!*args->paused) {
    WM.update();
  }
  WM.draw();
  DM.swapBuffers();
};

void GameManager::run() {
  long int adjustTime = 0;
  long int loopTime = 0;
  long int steps = 0;
  EventStep step(0);

  EmscriptenLoopArgs args = {&adjustTime, &loopTime, &steps,
                             clock.get(), frameTime, &this->paused};

  auto frames = Configuration::getMaxFrameRate();
  emscripten_set_main_loop_arg(loop, &args, frames, 1);
}
void GameManager::setGameOver(bool gameOver) {
  this->gameOver = gameOver;
  if (gameOver) {
    emscripten_cancel_main_loop();
  }
}
void GameManager::pause() { this->paused = true; }

void GameManager::resume() { this->paused = false; }

auto GameManager::isPaused() const -> bool { return this->paused; }
#endif

auto GameManager::getGameOver() const -> bool { return this->gameOver; }

auto GameManager::getFrameTime() const -> int { return this->frameTime; }
void GameManager::setFrameTime(int frameTime) { this->frameTime = frameTime; }

void GameManager::setRandomSeed(int seed) { srand(seed); }

}  // namespace lb