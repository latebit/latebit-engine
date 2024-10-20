#include "latebit/core/events/EventTarget.h"

#include <string>
#include <unordered_set>

#include "latebit/core/GameManager.h"
#include "latebit/core/input/InputManager.h"
#include "latebit/core/world/WorldManager.h"

namespace lb {
auto EventTarget::subscribe(std::string eventType) -> int {
  events.insert(eventType);

  if (IM.subscribe(this, eventType) == 0) {
    return 0;
  }

  if (GM.subscribe(this, eventType) == 0) {
    return 0;
  }

  // WM handles custom (user defined) events, hence it's the fallback
  return WM.subscribe(this, eventType);
}

auto EventTarget::unsubscribe(std::string eventType) -> int {
  events.erase(eventType);

  if (IM.unsubscribe(this, eventType) == 0) {
    return 0;
  }

  if (GM.unsubscribe(this, eventType) == 0) {
    return 0;
  }

  // WM handles custom (user defined) events, hence it's the fallback
  return WM.unsubscribe(this, eventType);
}

auto EventTarget::unsubscribeAll() -> int {
  auto result = 0;

  // Cannot use unsubscribe() here because it modifies the set while iterating
  for (auto& e : this->events) {
    result |= IM.unsubscribe(this, e);
    result |= GM.unsubscribe(this, e);
    result |= WM.unsubscribe(this, e);
  }

  this->events.clear();

  return result;
}

}  // namespace lb