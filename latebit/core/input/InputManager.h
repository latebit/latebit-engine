#pragma once

#include <SDL2/SDL_keycode.h>

#include "latebit/core/utils/Manager.h"

#define IM lb::InputManager::getInstance()

namespace lb {

namespace InputAction {
enum InputAction {
  UNDEFINED_ACTION = -1,
  PRESSED,
  RELEASED,
};
}

namespace InputKey {
enum InputKey {
  UNDEFINED_KEY = -1,
  A,
  B,
  L,
  R,
  START,
  OPTIONS,
  UP,
  DOWN,
  LEFT,
  RIGHT,
};
}

class InputManager : public Manager {
 private:
  // Singleton
  InputManager();

  // Converts SDL Keycode to InputKey.
  [[nodiscard]] auto fromSDLKeyCode(SDL_Keycode key) const
    -> InputKey::InputKey;

 public:
  static auto getInstance() -> InputManager &;
  InputManager(InputManager const &) = delete;
  void operator=(InputManager const &) = delete;

  auto startUp() -> int override;
  void shutDown() override;
  // Returns true if event is can be handled by this manager
  [[nodiscard]] auto isValid(string eventType) const -> bool override;

  // Polls for input from connected devices and dispatches EventInput
  // accordingly
  void getInput() const;
};
}  // namespace lb
