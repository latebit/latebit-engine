#include "InputManager.h"

#include <unordered_map>

#include "DisplayManager.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
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

  m_keyboardEvent[sf::Keyboard::Space] = Keyboard::SPACE;
  m_keyboardEvent[sf::Keyboard::Return] = Keyboard::RETURN;
  m_keyboardEvent[sf::Keyboard::Escape] = Keyboard::ESCAPE;
  m_keyboardEvent[sf::Keyboard::Tab] = Keyboard::TAB;
  m_keyboardEvent[sf::Keyboard::Left] = Keyboard::LEFTARROW;
  m_keyboardEvent[sf::Keyboard::Right] = Keyboard::RIGHTARROW;
  m_keyboardEvent[sf::Keyboard::Up] = Keyboard::UPARROW;
  m_keyboardEvent[sf::Keyboard::Down] = Keyboard::DOWNARROW;
  m_keyboardEvent[sf::Keyboard::Pause] = Keyboard::PAUSE;
  m_keyboardEvent[sf::Keyboard::Dash] = Keyboard::MINUS;
  m_keyboardEvent[sf::Keyboard::Equal] = Keyboard::PLUS;
  m_keyboardEvent[sf::Keyboard::Tilde] = Keyboard::TILDE;
  m_keyboardEvent[sf::Keyboard::Period] = Keyboard::PERIOD;
  m_keyboardEvent[sf::Keyboard::Comma] = Keyboard::COMMA;
  m_keyboardEvent[sf::Keyboard::Slash] = Keyboard::SLASH;
  m_keyboardEvent[sf::Keyboard::BackSlash] = Keyboard::BACKSLASH;
  m_keyboardEvent[sf::Keyboard::Multiply] = Keyboard::MULTIPLY;
  m_keyboardEvent[sf::Keyboard::Quote] = Keyboard::QUOTE;
  m_keyboardEvent[sf::Keyboard::SemiColon] = Keyboard::SEMICOLON;
  m_keyboardEvent[sf::Keyboard::LControl] = Keyboard::LEFTCONTROL;
  m_keyboardEvent[sf::Keyboard::RControl] = Keyboard::RIGHTCONTROL;
  m_keyboardEvent[sf::Keyboard::LShift] = Keyboard::LEFTSHIFT;
  m_keyboardEvent[sf::Keyboard::RShift] = Keyboard::RIGHTSHIFT;
  m_keyboardEvent[sf::Keyboard::F1] = Keyboard::F1;
  m_keyboardEvent[sf::Keyboard::F2] = Keyboard::F2;
  m_keyboardEvent[sf::Keyboard::F3] = Keyboard::F3;
  m_keyboardEvent[sf::Keyboard::F4] = Keyboard::F4;
  m_keyboardEvent[sf::Keyboard::F5] = Keyboard::F5;
  m_keyboardEvent[sf::Keyboard::F6] = Keyboard::F6;
  m_keyboardEvent[sf::Keyboard::F7] = Keyboard::F7;
  m_keyboardEvent[sf::Keyboard::F8] = Keyboard::F8;
  m_keyboardEvent[sf::Keyboard::F9] = Keyboard::F9;
  m_keyboardEvent[sf::Keyboard::F10] = Keyboard::F10;
  m_keyboardEvent[sf::Keyboard::F11] = Keyboard::F11;
  m_keyboardEvent[sf::Keyboard::F12] = Keyboard::F12;
  m_keyboardEvent[sf::Keyboard::A] = Keyboard::A;
  m_keyboardEvent[sf::Keyboard::B] = Keyboard::B;
  m_keyboardEvent[sf::Keyboard::C] = Keyboard::C;
  m_keyboardEvent[sf::Keyboard::D] = Keyboard::D;
  m_keyboardEvent[sf::Keyboard::E] = Keyboard::E;
  m_keyboardEvent[sf::Keyboard::F] = Keyboard::F;
  m_keyboardEvent[sf::Keyboard::G] = Keyboard::G;
  m_keyboardEvent[sf::Keyboard::H] = Keyboard::H;
  m_keyboardEvent[sf::Keyboard::I] = Keyboard::I;
  m_keyboardEvent[sf::Keyboard::J] = Keyboard::J;
  m_keyboardEvent[sf::Keyboard::K] = Keyboard::K;
  m_keyboardEvent[sf::Keyboard::L] = Keyboard::L;
  m_keyboardEvent[sf::Keyboard::M] = Keyboard::M;
  m_keyboardEvent[sf::Keyboard::N] = Keyboard::N;
  m_keyboardEvent[sf::Keyboard::O] = Keyboard::O;
  m_keyboardEvent[sf::Keyboard::P] = Keyboard::P;
  m_keyboardEvent[sf::Keyboard::Q] = Keyboard::Q;
  m_keyboardEvent[sf::Keyboard::R] = Keyboard::R;
  m_keyboardEvent[sf::Keyboard::S] = Keyboard::S;
  m_keyboardEvent[sf::Keyboard::T] = Keyboard::T;
  m_keyboardEvent[sf::Keyboard::U] = Keyboard::U;
  m_keyboardEvent[sf::Keyboard::V] = Keyboard::V;
  m_keyboardEvent[sf::Keyboard::W] = Keyboard::W;
  m_keyboardEvent[sf::Keyboard::X] = Keyboard::X;
  m_keyboardEvent[sf::Keyboard::Y] = Keyboard::Y;
  m_keyboardEvent[sf::Keyboard::Z] = Keyboard::Z;
  m_keyboardEvent[sf::Keyboard::Num0] = Keyboard::NUM0;
  m_keyboardEvent[sf::Keyboard::Num1] = Keyboard::NUM1;
  m_keyboardEvent[sf::Keyboard::Num2] = Keyboard::NUM2;
  m_keyboardEvent[sf::Keyboard::Num3] = Keyboard::NUM3;
  m_keyboardEvent[sf::Keyboard::Num4] = Keyboard::NUM4;
  m_keyboardEvent[sf::Keyboard::Num5] = Keyboard::NUM5;
  m_keyboardEvent[sf::Keyboard::Num6] = Keyboard::NUM6;
  m_keyboardEvent[sf::Keyboard::Num7] = Keyboard::NUM7;
  m_keyboardEvent[sf::Keyboard::Num8] = Keyboard::NUM8;
  m_keyboardEvent[sf::Keyboard::Num9] = Keyboard::NUM9;
  m_keyboardEvent[sf::Keyboard::BackSpace] = Keyboard::BACKSPACE;

  LM.writeLog("InputManager::InputManager(): Populated keyboard event map");

  m_mouseEvent[sf::Mouse::Left] = Mouse::LEFT;
  m_mouseEvent[sf::Mouse::Right] = Mouse::RIGHT;
  m_mouseEvent[sf::Mouse::Middle] = Mouse::MIDDLE;

  LM.writeLog("InputManager::InputManager(): Populated mouse event map");
}

int InputManager::startUp() {
  if (!DM.isStarted()) {
    LM.writeLog("InputManager::startUp(): DisplayManager is not started");
    return -1;
  }

  sf::RenderWindow* p_win = DM.getWindow();
  p_win->setKeyRepeatEnabled(false);
  LM.writeLog("InputManager::startUp(): Started successfully");
  return Manager::startUp();
}

void InputManager::shutDown() {
  DM.getWindow()->setKeyRepeatEnabled(true);
  Manager::shutDown();
  LM.writeLog("InputManager::shutDown(): Shut down successfully");
}

void InputManager::getInput() const {
  sf::Event event;
  auto win = DM.getWindow();

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
      default:
        break;
    }
  }
}

Keyboard::Key InputManager::fromSFMLKeyCode(sf::Keyboard::Key key) const {
  auto item = m_keyboardEvent.find(key);
  if (item != m_keyboardEvent.end()) {
    return item->second;
  }
  return Keyboard::UNDEFINED_KEY;
}

Mouse::Button InputManager::fromSFMLMouseButton(sf::Mouse::Button btn) const {
  auto item = m_mouseEvent.find(btn);
  if (item != m_mouseEvent.end()) {
    return item->second;
  }
  return Mouse::UNDEFINED_MOUSE_BUTTON;
}

}  // namespace df
