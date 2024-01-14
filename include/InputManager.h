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
  InputManager();
  InputManager(InputManager const &);
  void operator=(InputManager const &);
  std::unordered_map<sf::Keyboard::Key, Keyboard::Key> keyboardEvent;
  std::unordered_map<sf::Mouse::Button, Mouse::Button> mouseEvent;
  auto fromSFMLKeyCode(sf::Keyboard::Key key) const -> Keyboard::Key;
  auto fromSFMLMouseButton(sf::Mouse::Button btn) const -> Mouse::Button;

 public:
  static auto getInstance() -> InputManager &;
  auto startUp() -> int override;
  void shutDown() override;
  void getInput() const;
};
}  // namespace df
