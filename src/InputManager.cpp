#include "InputManager.h"

#include <unordered_map>

#include "DisplayManager.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "GameManager.h"
#include "LogManager.h"
#include "SFML/Window.hpp"

namespace df {

InputManager& InputManager::getInstance() {
  static InputManager instance;
  return instance;
}

InputManager::InputManager() {
  setType("InputManager");
  LM.writeLog("InputManager::InputManager(): Created InputManager");

  this->keyboardEvent[sf::Keyboard::Space] = Keyboard::SPACE;
  this->keyboardEvent[sf::Keyboard::Return] = Keyboard::RETURN;
  this->keyboardEvent[sf::Keyboard::Escape] = Keyboard::ESCAPE;
  this->keyboardEvent[sf::Keyboard::Tab] = Keyboard::TAB;
  this->keyboardEvent[sf::Keyboard::Left] = Keyboard::LEFTARROW;
  this->keyboardEvent[sf::Keyboard::Right] = Keyboard::RIGHTARROW;
  this->keyboardEvent[sf::Keyboard::Up] = Keyboard::UPARROW;
  this->keyboardEvent[sf::Keyboard::Down] = Keyboard::DOWNARROW;
  this->keyboardEvent[sf::Keyboard::Pause] = Keyboard::PAUSE;
  this->keyboardEvent[sf::Keyboard::Dash] = Keyboard::MINUS;
  this->keyboardEvent[sf::Keyboard::Equal] = Keyboard::PLUS;
  this->keyboardEvent[sf::Keyboard::Tilde] = Keyboard::TILDE;
  this->keyboardEvent[sf::Keyboard::Period] = Keyboard::PERIOD;
  this->keyboardEvent[sf::Keyboard::Comma] = Keyboard::COMMA;
  this->keyboardEvent[sf::Keyboard::Slash] = Keyboard::SLASH;
  this->keyboardEvent[sf::Keyboard::BackSlash] = Keyboard::BACKSLASH;
  this->keyboardEvent[sf::Keyboard::Multiply] = Keyboard::MULTIPLY;
  this->keyboardEvent[sf::Keyboard::Quote] = Keyboard::QUOTE;
  this->keyboardEvent[sf::Keyboard::SemiColon] = Keyboard::SEMICOLON;
  this->keyboardEvent[sf::Keyboard::LControl] = Keyboard::LEFTCONTROL;
  this->keyboardEvent[sf::Keyboard::RControl] = Keyboard::RIGHTCONTROL;
  this->keyboardEvent[sf::Keyboard::LShift] = Keyboard::LEFTSHIFT;
  this->keyboardEvent[sf::Keyboard::RShift] = Keyboard::RIGHTSHIFT;
  this->keyboardEvent[sf::Keyboard::F1] = Keyboard::F1;
  this->keyboardEvent[sf::Keyboard::F2] = Keyboard::F2;
  this->keyboardEvent[sf::Keyboard::F3] = Keyboard::F3;
  this->keyboardEvent[sf::Keyboard::F4] = Keyboard::F4;
  this->keyboardEvent[sf::Keyboard::F5] = Keyboard::F5;
  this->keyboardEvent[sf::Keyboard::F6] = Keyboard::F6;
  this->keyboardEvent[sf::Keyboard::F7] = Keyboard::F7;
  this->keyboardEvent[sf::Keyboard::F8] = Keyboard::F8;
  this->keyboardEvent[sf::Keyboard::F9] = Keyboard::F9;
  this->keyboardEvent[sf::Keyboard::F10] = Keyboard::F10;
  this->keyboardEvent[sf::Keyboard::F11] = Keyboard::F11;
  this->keyboardEvent[sf::Keyboard::F12] = Keyboard::F12;
  this->keyboardEvent[sf::Keyboard::A] = Keyboard::A;
  this->keyboardEvent[sf::Keyboard::B] = Keyboard::B;
  this->keyboardEvent[sf::Keyboard::C] = Keyboard::C;
  this->keyboardEvent[sf::Keyboard::D] = Keyboard::D;
  this->keyboardEvent[sf::Keyboard::E] = Keyboard::E;
  this->keyboardEvent[sf::Keyboard::F] = Keyboard::F;
  this->keyboardEvent[sf::Keyboard::G] = Keyboard::G;
  this->keyboardEvent[sf::Keyboard::H] = Keyboard::H;
  this->keyboardEvent[sf::Keyboard::I] = Keyboard::I;
  this->keyboardEvent[sf::Keyboard::J] = Keyboard::J;
  this->keyboardEvent[sf::Keyboard::K] = Keyboard::K;
  this->keyboardEvent[sf::Keyboard::L] = Keyboard::L;
  this->keyboardEvent[sf::Keyboard::M] = Keyboard::M;
  this->keyboardEvent[sf::Keyboard::N] = Keyboard::N;
  this->keyboardEvent[sf::Keyboard::O] = Keyboard::O;
  this->keyboardEvent[sf::Keyboard::P] = Keyboard::P;
  this->keyboardEvent[sf::Keyboard::Q] = Keyboard::Q;
  this->keyboardEvent[sf::Keyboard::R] = Keyboard::R;
  this->keyboardEvent[sf::Keyboard::S] = Keyboard::S;
  this->keyboardEvent[sf::Keyboard::T] = Keyboard::T;
  this->keyboardEvent[sf::Keyboard::U] = Keyboard::U;
  this->keyboardEvent[sf::Keyboard::V] = Keyboard::V;
  this->keyboardEvent[sf::Keyboard::W] = Keyboard::W;
  this->keyboardEvent[sf::Keyboard::X] = Keyboard::X;
  this->keyboardEvent[sf::Keyboard::Y] = Keyboard::Y;
  this->keyboardEvent[sf::Keyboard::Z] = Keyboard::Z;
  this->keyboardEvent[sf::Keyboard::Num0] = Keyboard::NUM0;
  this->keyboardEvent[sf::Keyboard::Num1] = Keyboard::NUM1;
  this->keyboardEvent[sf::Keyboard::Num2] = Keyboard::NUM2;
  this->keyboardEvent[sf::Keyboard::Num3] = Keyboard::NUM3;
  this->keyboardEvent[sf::Keyboard::Num4] = Keyboard::NUM4;
  this->keyboardEvent[sf::Keyboard::Num5] = Keyboard::NUM5;
  this->keyboardEvent[sf::Keyboard::Num6] = Keyboard::NUM6;
  this->keyboardEvent[sf::Keyboard::Num7] = Keyboard::NUM7;
  this->keyboardEvent[sf::Keyboard::Num8] = Keyboard::NUM8;
  this->keyboardEvent[sf::Keyboard::Num9] = Keyboard::NUM9;
  this->keyboardEvent[sf::Keyboard::BackSpace] = Keyboard::BACKSPACE;

  LM.writeLog("InputManager::InputManager(): Populated keyboard event map");

  this->mouseEvent[sf::Mouse::Left] = Mouse::LEFT;
  this->mouseEvent[sf::Mouse::Right] = Mouse::RIGHT;
  this->mouseEvent[sf::Mouse::Middle] = Mouse::MIDDLE;

  LM.writeLog("InputManager::InputManager(): Populated mouse event map");
}

int InputManager::startUp() {
  if (!DM.isStarted()) {
    LM.writeLog("InputManager::startUp(): DisplayManager is not started");
    return -1;
  }

  sf::RenderWindow* win = DM.getWindow();
  win->setKeyRepeatEnabled(false);
  LM.writeLog("InputManager::startUp(): Started successfully");
  return Manager::startUp();
}

void InputManager::shutDown() {
  auto win = DM.getWindow();
  if (win != nullptr) win->setKeyRepeatEnabled(true);

  Manager::shutDown();
  LM.writeLog("InputManager::shutDown(): Shut down successfully");
}

void InputManager::getInput() const {
  sf::Event event;
  auto win = DM.getWindow();

  if (win == nullptr) return;

  while (win->pollEvent(event)) {
    Keyboard::Key key;
    Mouse::Button btn;
    Vector pos;

    switch (event.type) {
      case sf::Event::KeyPressed:
        key = fromSFMLKeyCode(event.key.code);
        onEvent(
          new df::EventKeyboard(key, df::EventKeyboardAction::KEY_PRESSED));
        break;
      case sf::Event::KeyReleased:
        key = fromSFMLKeyCode(event.key.code);
        onEvent(
          new df::EventKeyboard(key, df::EventKeyboardAction::KEY_RELEASED));
        break;
      case sf::Event::MouseMoved:
        pos = Vector(event.mouseMove.x, event.mouseMove.y);
        btn = fromSFMLMouseButton(event.mouseButton.button);
        onEvent(new df::EventMouse(df::EventMouseAction::MOVED, btn, pos));
        break;
      case sf::Event::MouseButtonPressed:
        pos = Vector(event.mouseMove.x, event.mouseMove.y);
        btn = fromSFMLMouseButton(event.mouseButton.button);
        onEvent(new df::EventMouse(df::EventMouseAction::CLICKED, btn, pos));
        break;
      case sf::Event::Closed:
        GM.shutDown();
        // This is meant to break out of the while loop
        return;
      default:
        break;
    }
  }
}

Keyboard::Key InputManager::fromSFMLKeyCode(sf::Keyboard::Key key) const {
  auto item = this->keyboardEvent.find(key);
  if (item != this->keyboardEvent.end()) {
    return item->second;
  }
  return Keyboard::UNDEFINED_KEY;
}

Mouse::Button InputManager::fromSFMLMouseButton(sf::Mouse::Button btn) const {
  auto item = this->mouseEvent.find(btn);
  if (item != this->mouseEvent.end()) {
    return item->second;
  }
  return Mouse::UNDEFINED_MOUSE_BUTTON;
}

}  // namespace df
