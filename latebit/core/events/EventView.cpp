#include "EventView.h"

namespace lb {

auto EventView::getObjectType() const -> string { return objectType; }

auto EventView::getValue() const -> int { return value; }

auto EventView::getDelta() const -> bool { return delta; }

}  // namespace lb