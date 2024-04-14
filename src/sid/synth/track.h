#pragma once

#include <vector>

#include "oscillator.h"
#include "utils.h"

namespace sid {
// A Note is a musical symbol that represents a sound. It has a pitch, a volume,
// a wave type and an effect type. It also has a unique identifier that can be
// used to compare notes.
// A note can also be a rest, which is a symbol that represents silence.
// A note can also be invalid, which is a symbol that represents an invalid note
// and it is used to report errors.
class Note {
 public:
  Note(sid_byte pitch, sid_hex volume, WaveType wave, EffectType effect);
  // Creates a new rest note
  static auto rest() -> Note;
  // Creates a new invalid note
  static auto invalid() -> Note;

  // Checks if the given note is a rest
  auto isRest() -> bool;
  // Checks if the given notes are the same (i.e., have the same id)
  auto isSame(Note other) -> bool;
  // Checks if the given notes are equal (i.e., same pitch, volume, wave)
  auto isEqual(Note other) -> bool;
  // Checks if the given note is invalid
  auto isInvalid() -> bool;

 private:
  // A number between 0 and 96 describing a note from C0 to B7
  sid_byte pitch = 0;
  // A number between 0 and 15 describing the volume of the note
  sid_hex volume = 0;
  // The type of wave associated with the note
  WaveType wave = TRIANGLE;
  // The type of effect associated with the note
  EffectType effect = NONE;
  // The unique identifier of the note, used to compare notes
  long unsigned int id = 0;
};

using Track = std::vector<Note>;

}  // namespace sid
