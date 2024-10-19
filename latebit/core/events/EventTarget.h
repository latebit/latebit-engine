#pragma once

#include <string>
#include <unordered_set>
#include "latebit/core/events/Event.h"

namespace lb {
class EventTarget {
private:
    unordered_set<string> events = {};
public:
    EventTarget() = default;
    virtual ~EventTarget() = default;
    virtual auto eventHandler([[maybe_unused]] const Event* e) -> int { return 0; };
    auto subscribe(string eventType) -> int;
    auto unsubscribe(std::string eventType) -> int;
    auto unsubscribeAll() -> int;
    [[nodiscard]] virtual auto isActive() const -> bool { return true; };
};
}