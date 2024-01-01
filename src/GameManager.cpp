#include "GameManager.h"

#include "EventStep.h"
#include "LogManager.h"
#include "ObjectListIterator.h"
#include "WorldManager.h"
#include "utils.h"

namespace df {

GameManager::GameManager() {}

int GameManager::startUp(int frame_time) {
  setType("GameManager");
  LM.startUp();
  WM.startUp();

  frame_time = frame_time;
  game_over = false;
  p_clock = new Clock;
  LM.writeLog("(GameManager::startUp) game_over=%b, frame_time=%d", game_over,
              frame_time);
  return 0;
}

GameManager& GameManager::getInstance() {
  static GameManager instance;
  return instance;
}

void GameManager::shutDown() {
  setGameOver(true);
  LM.shutDown();
}

void GameManager::run() {
  long int adjust_time = 0;
  long int loop_time = 0;

  while (!game_over) {
    p_clock->delta();

    // Send a step event to all Objects
    onEvent(new EventStep);

    // Update the World
    WM.update();

    loop_time = p_clock->split();
    p_clock->delta();
    sleep(frame_time - loop_time);
  }
}

void GameManager::setGameOver(bool new_game_over) { game_over = new_game_over; }

bool GameManager::getGameOver() const { return game_over; }

int GameManager::getFrameTime() const { return frame_time; }

}  // namespace df