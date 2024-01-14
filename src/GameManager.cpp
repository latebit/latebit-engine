#include "GameManager.h"

#include "DisplayManager.h"
#include "EventStep.h"
#include "InputManager.h"
#include "LogManager.h"
#include "ObjectListIterator.h"
#include "WorldManager.h"
#include "utils.h"

namespace df {

GameManager::GameManager() {
  setType("GameManager");
  frame_time = FRAME_TIME_DEFAULT;
  game_over = false;
  clock = new Clock;
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

  frame_time = ft;

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

void GameManager::run() {
  long int adjust_time = 0;
  long int lootime = 0;
  long int steps = 0;

  while (!game_over) {
    clock->delta();

    // Send a step event to all Objects
    onEvent(new EventStep(++steps));

    IM.getInput();
    WM.update();
    WM.draw();
    DM.swapBuffers();

    lootime = clock->delta();
    sleep(frame_time - lootime);
  }
}

void GameManager::setGameOver(bool new_game_over) { game_over = new_game_over; }

auto GameManager::getGameOver() const -> bool { return game_over; }

auto GameManager::getFrameTime() const -> int { return frame_time; }

}  // namespace df