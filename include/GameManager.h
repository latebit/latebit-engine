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
  Clock *clock;

 public:
  static auto getInstance() -> GameManager &;

  auto startUp(int frame_time = FRAME_TIME_DEFAULT) -> int;

  void shutDown() override;

  void run();

  void setGameOver(bool new_game_over = true);

  auto getGameOver() const -> bool;

  auto getFrameTime() const -> int;
};
}  // namespace df

#define GM df::GameManager::getInstance()
