#include "InputManager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <unordered_map>

#include "core/GameManager.h"
#include "core/events/EventKeyboard.h"
#include "core/events/EventMouse.h"
#include "core/graphics/DisplayManager.h"
#include "core/utils/Logger.h"

namespace lb {

auto InputManager::getInstance() -> InputManager& {
  static InputManager instance;
  return instance;
}

InputManager::InputManager() {
  setType("InputManager");
  Log.debug("InputManager::InputManager(): Created InputManager");

  this->keyboardEvent[SDLK_SPACE] = Keyboard::SPACE;
  this->keyboardEvent[SDLK_RETURN] = Keyboard::RETURN;
  this->keyboardEvent[SDLK_ESCAPE] = Keyboard::ESCAPE;
  this->keyboardEvent[SDLK_TAB] = Keyboard::TAB;
  this->keyboardEvent[SDLK_LEFT] = Keyboard::LEFTARROW;
  this->keyboardEvent[SDLK_RIGHT] = Keyboard::RIGHTARROW;
  this->keyboardEvent[SDLK_UP] = Keyboard::UPARROW;
  this->keyboardEvent[SDLK_DOWN] = Keyboard::DOWNARROW;
  this->keyboardEvent[SDLK_PAUSE] = Keyboard::PAUSE;
  this->keyboardEvent[SDLK_MINUS] = Keyboard::MINUS;
  this->keyboardEvent[SDLK_PLUS] = Keyboard::PLUS;
  this->keyboardEvent[SDLK_PERIOD] = Keyboard::PERIOD;
  this->keyboardEvent[SDLK_COMMA] = Keyboard::COMMA;
  this->keyboardEvent[SDLK_SLASH] = Keyboard::SLASH;
  this->keyboardEvent[SDLK_BACKSLASH] = Keyboard::BACKSLASH;
  this->keyboardEvent[SDLK_ASTERISK] = Keyboard::MULTIPLY;
  this->keyboardEvent[SDLK_QUOTE] = Keyboard::QUOTE;
  this->keyboardEvent[SDLK_SEMICOLON] = Keyboard::SEMICOLON;
  this->keyboardEvent[SDLK_LCTRL] = Keyboard::LEFTCONTROL;
  this->keyboardEvent[SDLK_RCTRL] = Keyboard::RIGHTCONTROL;
  this->keyboardEvent[SDLK_LSHIFT] = Keyboard::LEFTSHIFT;
  this->keyboardEvent[SDLK_RSHIFT] = Keyboard::RIGHTSHIFT;
  this->keyboardEvent[SDLK_F1] = Keyboard::F1;
  this->keyboardEvent[SDLK_F2] = Keyboard::F2;
  this->keyboardEvent[SDLK_F3] = Keyboard::F3;
  this->keyboardEvent[SDLK_F4] = Keyboard::F4;
  this->keyboardEvent[SDLK_F5] = Keyboard::F5;
  this->keyboardEvent[SDLK_F6] = Keyboard::F6;
  this->keyboardEvent[SDLK_F7] = Keyboard::F7;
  this->keyboardEvent[SDLK_F8] = Keyboard::F8;
  this->keyboardEvent[SDLK_F9] = Keyboard::F9;
  this->keyboardEvent[SDLK_F10] = Keyboard::F10;
  this->keyboardEvent[SDLK_F11] = Keyboard::F11;
  this->keyboardEvent[SDLK_F12] = Keyboard::F12;
  this->keyboardEvent[SDLK_a] = Keyboard::A;
  this->keyboardEvent[SDLK_b] = Keyboard::B;
  this->keyboardEvent[SDLK_c] = Keyboard::C;
  this->keyboardEvent[SDLK_d] = Keyboard::D;
  this->keyboardEvent[SDLK_e] = Keyboard::E;
  this->keyboardEvent[SDLK_f] = Keyboard::F;
  this->keyboardEvent[SDLK_g] = Keyboard::G;
  this->keyboardEvent[SDLK_h] = Keyboard::H;
  this->keyboardEvent[SDLK_i] = Keyboard::I;
  this->keyboardEvent[SDLK_j] = Keyboard::J;
  this->keyboardEvent[SDLK_k] = Keyboard::K;
  this->keyboardEvent[SDLK_l] = Keyboard::L;
  this->keyboardEvent[SDLK_m] = Keyboard::M;
  this->keyboardEvent[SDLK_n] = Keyboard::N;
  this->keyboardEvent[SDLK_o] = Keyboard::O;
  this->keyboardEvent[SDLK_p] = Keyboard::P;
  this->keyboardEvent[SDLK_q] = Keyboard::Q;
  this->keyboardEvent[SDLK_r] = Keyboard::R;
  this->keyboardEvent[SDLK_s] = Keyboard::S;
  this->keyboardEvent[SDLK_t] = Keyboard::T;
  this->keyboardEvent[SDLK_u] = Keyboard::U;
  this->keyboardEvent[SDLK_v] = Keyboard::V;
  this->keyboardEvent[SDLK_w] = Keyboard::W;
  this->keyboardEvent[SDLK_x] = Keyboard::X;
  this->keyboardEvent[SDLK_y] = Keyboard::Y;
  this->keyboardEvent[SDLK_z] = Keyboard::Z;
  this->keyboardEvent[SDLK_0] = Keyboard::NUM0;
  this->keyboardEvent[SDLK_1] = Keyboard::NUM1;
  this->keyboardEvent[SDLK_2] = Keyboard::NUM2;
  this->keyboardEvent[SDLK_3] = Keyboard::NUM3;
  this->keyboardEvent[SDLK_4] = Keyboard::NUM4;
  this->keyboardEvent[SDLK_5] = Keyboard::NUM5;
  this->keyboardEvent[SDLK_6] = Keyboard::NUM6;
  this->keyboardEvent[SDLK_7] = Keyboard::NUM7;
  this->keyboardEvent[SDLK_8] = Keyboard::NUM8;
  this->keyboardEvent[SDLK_9] = Keyboard::NUM9;
  this->keyboardEvent[SDLK_BACKSPACE] = Keyboard::BACKSPACE;

  Log.debug("InputManager::InputManager(): Populated keyboard event map");
}

auto InputManager::startUp() -> int {
  if (!DM.isStarted()) {
    Log.error(
      "InputManager::startUp(): Cannot start. DisplayManager is not started");
    return -1;
  }

  Log.info("InputManager::startUp(): Started successfully");
  return Manager::startUp();
}

void InputManager::shutDown() {
  Manager::shutDown();
  Log.info("InputManager::shutDown(): Shut down successfully");
}

auto InputManager::isValid(string eventType) const -> bool {
  return eventType == lb::KEYBOARD_EVENT || eventType == lb::MSE_EVENT;
}

void InputManager::getInput() const {
  SDL_Event event;
  EventKeyboard keyEvent;
  EventMouse mouseEvent;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        keyEvent.setKey(fromSDLKeyCode(event.key.keysym.sym));
        keyEvent.setKeyboardAction(lb::EventKeyboardAction::KEY_PRESSED);
        onEvent(&keyEvent);
        break;
      case SDL_KEYUP:
        keyEvent.setKey(fromSDLKeyCode(event.key.keysym.sym));
        keyEvent.setKeyboardAction(lb::EventKeyboardAction::KEY_RELEASED);
        onEvent(&keyEvent);
        break;
      case SDL_MOUSEMOTION:
        mouseEvent.setMouseAction(lb::EventMouseAction::MOVED);
        mouseEvent.setMousePosition(
          pixelsToCells(Vector(event.motion.x, event.motion.y)));
        onEvent(&mouseEvent);
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseEvent.setMouseAction(lb::EventMouseAction::CLICKED);
        mouseEvent.setMouseButton(fromSDLMouseButton(event.button.button));
        mouseEvent.setMousePosition(
          pixelsToCells(Vector(event.motion.x, event.motion.y)));
        onEvent(&mouseEvent);
        break;
      case SDL_QUIT:
        GM.setGameOver();
        // This is meant to break out of the while loop
        return;
      default:
        break;
    }
  }
}

auto InputManager::fromSDLKeyCode(SDL_Keycode key) const -> Keyboard::Key {
  auto item = this->keyboardEvent.find(key);
  if (item != this->keyboardEvent.end()) {
    return item->second;
  }
  return Keyboard::UNDEFINED_KEY;
}

auto InputManager::fromSDLMouseButton(char btn) const -> Mouse::Button {
  switch (btn) {
    case SDL_BUTTON_LEFT:
      return Mouse::LEFT;
    case SDL_BUTTON_RIGHT:
      return Mouse::RIGHT;
    case SDL_BUTTON_MIDDLE:
      return Mouse::MIDDLE;
  }
  return Mouse::UNDEFINED_MOUSE_BUTTON;
}

}  // namespace lb
