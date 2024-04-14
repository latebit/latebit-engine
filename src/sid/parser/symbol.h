#include <string>

#include "sid/synth/track.h"

using namespace std;

namespace sid {

// Symbol is a string that represents a Note in the SID language.
using Symbol = string;

auto toNote(const Symbol &symbol) -> Note;
auto isRest(const Symbol &symbol) -> bool;
auto isContinue(const Symbol &symbol) -> bool;
auto isEndOfTrack(const Symbol &symbol) -> bool;

};  // namespace sid
