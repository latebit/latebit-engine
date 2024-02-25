#pragma once

#include <cstddef>
#include <ctime>

#include "Clock.h"
#include "EventStep.h"
#include "Manager.h"

using namespace std;

namespace lb {

// duration of a frame in microseconds
const int FRAME_TIME_DEFAULT = 33333;  // ~30fps

class GameManager : public Manager {
 private:
  GameManager();

  // True when game loop is done
  bool gameOver = false;
  // Duration of a frame in microseconds
  int frameTime = FRAME_TIME_DEFAULT;

  // A reference to the game loop clock
  Clock *clock = new Clock;

 public:
  GameManager(GameManager const &) = delete;
  void operator=(GameManager const &) = delete;
  static auto getInstance() -> GameManager &;

  auto startUp() -> int override;
  void shutDown() override;
  [[nodiscard]] auto isValid(string eventType) const -> bool override;

  // Run game loop
  void run();

  // Set game over flag to stop the loop
  void setGameOver(bool gameOver = true);
  // Get game over flag
  [[nodiscard]] auto getGameOver() const -> bool;

  // Set duration of a frame in microseconds
  void setFrameTime(int frameTime);
  // Get duration of a frame in microseconds
  [[nodiscard]] auto getFrameTime() const -> int;

  // Define random starting seed. Better to be called before startUp().
  void setRandomSeed(int seed = time(nullptr));

#ifdef __EMSCRIPTEN__
  static void loop(void *arg);
#endif
};
}  // namespace lb

#define GM lb::GameManager::getInstance()
