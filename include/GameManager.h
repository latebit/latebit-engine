#pragma once

#include "Clock.h"
#include "Manager.h"

namespace df {

// duration of a frame in microseconds
const int FRAME_TIME_DEFAULT = 33333;  // ~30fps

class GameManager : public Manager {
 private:
  GameManager();
  GameManager(GameManager const &);
  void operator=(GameManager const &);
  bool game_over;
  int frame_time;
  Clock *p_clock;

 public:
  static GameManager &getInstance();

  int startUp(int frame_time = FRAME_TIME_DEFAULT);

  void shutDown();

  void run();

  void setGameOver(bool new_game_over = true);

  bool getGameOver() const;

  int getFrameTime() const;
};
}  // namespace df

#define GM df::GameManager::getInstance()

