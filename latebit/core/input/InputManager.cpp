#include "InputManager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <unordered_map>

#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/utils/Logger.h"

namespace lb {

auto InputManager::getInstance() -> InputManager& {
  static InputManager instance;
  return instance;
}

InputManager::InputManager() : Manager("InputManager") {
  Log.debug("InputManager::InputManager(): Created InputManager");
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
  return eventType == lb::INPUT_EVENT;
}

void InputManager::getInput() const {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN: {
        const auto key = fromSDLKeyCode(event.key.keysym.sym);
        if (key == InputKey::UNDEFINED_KEY) return;

        const auto evt = EventInput(key, InputAction::PRESSED);
        onEvent(&evt);
        break;
      }
      case SDL_KEYUP: {
        const auto key = fromSDLKeyCode(event.key.keysym.sym);
        if (key == InputKey::UNDEFINED_KEY) return;

        const auto evt = EventInput(key, InputAction::RELEASED);
        onEvent(&evt);
        break;
      }
      case SDL_QUIT:
        GM.setGameOver();
        // This is meant to break out of the while loop
        return;
      default:
        break;
    }
  }
}

auto InputManager::fromSDLKeyCode(SDL_Keycode key) const -> InputKey::InputKey {
  switch (key) {
    case SDLK_z:
      return InputKey::A;
    case SDLK_x:
      return InputKey::B;
    case SDLK_a:
      return InputKey::L;
    case SDLK_s:
      return InputKey::R;
    case SDLK_UP:
      return InputKey::UP;
    case SDLK_DOWN:
      return InputKey::DOWN;
    case SDLK_LEFT:
      return InputKey::LEFT;
    case SDLK_RIGHT:
      return InputKey::RIGHT;
    case SDLK_RETURN:
    case SDLK_RETURN2:
      return InputKey::START;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
      return InputKey::OPTIONS;
    default:
      return InputKey::UNDEFINED_KEY;
  }
  return InputKey::UNDEFINED_KEY;
}

}  // namespace lb
