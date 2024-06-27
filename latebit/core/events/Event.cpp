#include "Event.h"

using namespace std;

namespace lb {
auto Event::getType() const -> string { return this->type; }
}  // namespace lb
