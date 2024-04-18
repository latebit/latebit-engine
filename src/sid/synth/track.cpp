#include "track.h"

namespace sid {

int id = 1;

Note::Note(int pitch, int volume, WaveType wave, EffectType effect)
  : pitch(pitch), volume(volume), wave(wave), effect(effect) {
  this->id = id++;
}

auto Note::rest() -> Note {
  auto note = Note(0, 0, TRIANGLE, NONE);
  note.id = -1;
  return note;
}

auto Note::invalid() -> Note {
  auto note = Note(0, 0, TRIANGLE, NONE);
  note.id = -2;
  return note;
}

auto Note::isRest() -> bool { return this->id == -1; }
auto Note::isSame(Note other) -> bool { return this->id == other.id; }
auto Note::isEqual(Note other) -> bool {
  return this->pitch == other.pitch && this->volume == other.volume &&
         this->wave == other.wave && this->effect == other.effect;
}
auto Note::isInvalid() -> bool { return this->id == -2; }

auto Note::getPitch() -> int { return this->pitch; }
auto Note::getVolume() -> float { return this->volume / 15.0; }
auto Note::getWave() -> WaveType { return this->wave; }
auto Note::getEffect() -> EffectType { return this->effect; }

}  // namespace sid