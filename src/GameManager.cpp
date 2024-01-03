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
  p_clock = new Clock;
  LM.writeLog("GameManager::GameManager(): Created GameManager");
}

int GameManager::startUp(int frame_time) {
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

  frame_time = frame_time;

  LM.writeLog("GameManager::startUp(): Started successfully");
  return Manager::startUp();
}

GameManager& GameManager::getInstance() {
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
  long int loop_time = 0;

  while (!game_over) {
    p_clock->delta();

    // Send a step event to all Objects
    onEvent(new EventStep);

    IM.getInput();
    WM.update();
    WM.draw();
    DM.swapBuffers();

    loop_time = p_clock->split();
    p_clock->delta();
    sleep(frame_time - loop_time);
  }
}

void GameManager::setGameOver(bool new_game_over) { game_over = new_game_over; }

bool GameManager::getGameOver() const { return game_over; }

int GameManager::getFrameTime() const { return frame_time; }

}  // namespace df