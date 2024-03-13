#include "GameManager.h"

#include "AudioManager.h"
#include "Configuration.h"
#include "DisplayManager.h"
#include "EventStep.h"
#include "InputManager.h"
#include "Logger.h"
#include "WorldManager.h"
#include "utils.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace lb {

GameManager::GameManager() {
  setType("GameManager");
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
  Box boundary(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  WM.setBoundary(boundary);
  WM.setView(boundary);

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

  EventStep step(0);

  while (!gameOver) {
    clock->delta();

    // Send a step event to all subscribers
    step.setStepCount(++steps);
    onEvent(&step);

    IM.getInput();
    WM.update();
    WM.draw();
    DM.swapBuffers();

    loopTime = clock->delta();
    sleep(frameTime - loopTime);
  }
}
void GameManager::setGameOver(bool gameOver) { this->gameOver = gameOver; }
#endif

#ifdef __EMSCRIPTEN__
struct EmscriptenLoopArgs {
  long int* adjustTime = 0;
  long int* loopTime = 0;
  long int* steps = 0;
  EventStep* step = 0;
  Clock* clock = 0;
  int frameTime = 0;
};

void GameManager::loop(void* a) {
  EmscriptenLoopArgs* args = (EmscriptenLoopArgs*)a;
  args->step->setStepCount(++(*args->steps));
  GM.onEvent(args->step);

  IM.getInput();
  WM.update();
  WM.draw();
  DM.swapBuffers();
};

void GameManager::run() {
  long int adjustTime = 0;
  long int loopTime = 0;
  long int steps = 0;
  EventStep step(0);

  EmscriptenLoopArgs args = {&adjustTime, &loopTime, &steps,
                             &step,       clock,     frameTime};

  emscripten_set_main_loop_arg(loop, &args, 0, 1);
  emscripten_set_main_loop_timing(EM_TIMING_RAF, 33);
}
void GameManager::setGameOver(bool gameOver) {
  this->gameOver = gameOver;
  if (gameOver) {
    emscripten_cancel_main_loop();
  }
}
#endif

auto GameManager::getGameOver() const -> bool { return this->gameOver; }

auto GameManager::getFrameTime() const -> int { return this->frameTime; }
void GameManager::setFrameTime(int frameTime) { this->frameTime = frameTime; }

void GameManager::setRandomSeed(int seed) { srand(seed); }

}  // namespace lb