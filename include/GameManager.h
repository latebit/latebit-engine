#pragma once

#include "Clock.h"
#include "Manager.h"

using namespace std;

namespace df {

// duration of a frame in microseconds
const int FRAME_TIME_DEFAULT = 33333;  // ~30fps

class GameManager : public Manager {
 private:
  GameManager();
  GameManager(GameManager const &);
  void operator=(GameManager const &);

  // True when game loop is done
  bool gameOver = false;
  // Duration of a frame in microseconds
  int frameTime = FRAME_TIME_DEFAULT;

  // A reference to the game loop clock
  Clock *clock = new Clock;

 public:
  static auto getInstance() -> GameManager &;

  auto startUp(int frame_time = FRAME_TIME_DEFAULT) -> int;
  void shutDown() override;
  [[nodiscard]] auto isValid(string eventType) const -> bool override;

  // Run game loop
  void run();

  // Set game over flag to stop the loop
  void setGameOver(bool gameOver = true);
  // Get game over flag
  [[nodiscard]] auto getGameOver() const -> bool;

  // Get duration of a frame in microseconds
  [[nodiscard]] auto getFrameTime() const -> int;
};
}  // namespace df

#define GM df::GameManager::getInstance()
