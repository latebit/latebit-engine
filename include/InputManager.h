#pragma once

#include <unordered_map>

#include "EventKeyboard.h"
#include "EventMouse.h"
#include "Manager.h"
#include "SFML/Window.hpp"

#define IM df::InputManager::getInstance()

namespace df {
class InputManager : public Manager {
 private:
  // Singleton
  InputManager();
  InputManager(InputManager const &);
  void operator=(InputManager const &);

  // Map for quick lookup of keyboard events.
  std::unordered_map<sf::Keyboard::Key, Keyboard::Key> keyboardEvent;

  // Map for quick lookup of mouse events.
  std::unordered_map<sf::Mouse::Button, Mouse::Button> mouseEvent;

  // Converts SFML key code to local key code.
  auto fromSFMLKeyCode(sf::Keyboard::Key key) const -> Keyboard::Key;

  // Converts SFML mouse buttons to local mouse button.
  auto fromSFMLMouseButton(sf::Mouse::Button btn) const -> Mouse::Button;

 public:
  static auto getInstance() -> InputManager &;
  auto startUp() -> int override;
  void shutDown() override;
  [[nodiscard]] auto isValid(string eventType) const -> bool override;

  // Get input from the keyboard and mouse.
  void getInput() const;
};
}  // namespace df
