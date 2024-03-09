#pragma once

#include <SDL2/SDL_keycode.h>

#include <unordered_map>

#include "EventKeyboard.h"
#include "EventMouse.h"
#include "Manager.h"

#define IM lb::InputManager::getInstance()

namespace lb {
class InputManager : public Manager {
 private:
  // Singleton
  InputManager();

  // Map for quick lookup of keyboard events.
  std::unordered_map<SDL_Keycode, Keyboard::Key> keyboardEvent;

  // Converts SDL key code to local key code.
  [[nodiscard]] auto fromSDLKeyCode(SDL_Keycode key) const -> Keyboard::Key;

  // Converts SDL mouse buttons to local mouse button.
  [[nodiscard]] auto fromSDLMouseButton(char btn) const -> Mouse::Button;

 public:
  static auto getInstance() -> InputManager &;
  InputManager(InputManager const &) = delete;
  void operator=(InputManager const &) = delete;

  auto startUp() -> int override;
  void shutDown() override;
  // Returns true if event is can be handled by this manager
  [[nodiscard]] auto isValid(string eventType) const -> bool override;

  // Get input from the keyboard and mouse.
  void getInput() const;
};
}  // namespace lb
