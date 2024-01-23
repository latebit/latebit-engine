#include "GameManager.h"

#include "DisplayManager.h"
#include "EventStep.h"
#include "InputManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "utils.h"

namespace df {

GameManager::GameManager() {
  setType("GameManager");
  LM.writeLog("GameManager::GameManager(): Created GameManager");
}

auto GameManager::startUp(int ft) -> int {
  if (LM.startUp() != 0) {
    printf("GameManager::startUp(): Error starting LogManager\n");
    return -1;
  }

  if (WM.startUp() != 0) {
    LM.writeLog("GameManager::startUp(): Error starting WorldManager");
    return -1;
  }

  if (DM.startUp() != 0) {
    LM.writeLog("GameManager::startUp(): Error starting DisplayManager");
    return -1;
  }

  if (IM.startUp() != 0) {
    LM.writeLog("GameManager::startUp(): Error starting InputManager");
    return -1;
  }

  // By default boundary equates view and it's the whole window
  Box boundary(Vector(0, 0), DM.getHorizontalCells(), DM.getVerticalCells());
  WM.setBoundary(boundary);
  WM.setView(boundary);

  frameTime = ft;

  LM.writeLog("GameManager::startUp(): Started successfully");
  return Manager::startUp();
}

auto GameManager::getInstance() -> GameManager& {
  static GameManager instance;
  return instance;
}

void GameManager::shutDown() {
  setGameOver(true);
  IM.shutDown();
  DM.shutDown();
  WM.shutDown();
  Manager::shutDown();
  LM.writeLog("GameManager::shutDown(): Shut down successfully");
  LM.shutDown();
}

auto GameManager::isValid(string eventType) const -> bool {
  return eventType == STEP_EVENT;
}

void GameManager::run() {
  long int adjustTime = 0;
  long int loopTime = 0;
  long int steps = 0;

  EventStep step(0);

  while (!gameOver) {
    clock->delta();

    // Send a step event to all Objects
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

auto GameManager::getGameOver() const -> bool { return this->gameOver; }

auto GameManager::getFrameTime() const -> int { return this->frameTime; }

}  // namespace df