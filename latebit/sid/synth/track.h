#pragma once

#include <string>
#include <vector>

#include "oscillator.h"

using namespace std;

namespace sid {
// Symbol is a string that represents a Note. It can be a rest, a continue,
// the end of a track or a note. It is used to parse and generate notes.
using Symbol = string;

const string REST_SYMBOL = "------";
const string CONTINUE_SYMBOL = "......";
const string END_OF_TRACK_SYMBOL = "      ";

// A Note is a musical object that represents a sound. It has a pitch, a volume,
// a wave type and an effect type. It also has a unique identifier that can be
// used to compare notes. Notes are immutable, which means that once they are
// created, they cannot be modified.
// A note can also be a rest, which is a symbol that represents silence.
// A note can also be invalid, which is a symbol that represents an invalid note
// and it is used to report errors.
class Note {
 public:
  // Creates a new rest note
  static auto makeRest() -> Note;
  // Creates a new invalid note
  static auto makeInvalid() -> Note;
  // Creates a new continue note
  static auto makeContinue() -> Note;
  // Creates a new note from a symbol
  static auto fromSymbol(const Symbol& str) -> Note;

  // Checks if the given note is a rest
  [[nodiscard]] auto isRest() const -> bool;
  // Checks if the given notes are equal (i.e., same pitch, volume, wave)
  [[nodiscard]] auto isEqual(Note other) const -> bool;
  // Checks if the given note is invalid
  [[nodiscard]] auto isInvalid() const -> bool;
  // Checks if the given note is continue
  [[nodiscard]] auto isContinue() const -> bool;

  // Returns the pitch of the note
  [[nodiscard]] auto getPitch() const -> int;
  // Returns the volume of the note in the range [0, 1]
  [[nodiscard]] auto getVolume() const -> float;
  // Returns the wave type of the note
  [[nodiscard]] auto getWave() const -> WaveType;
  // Returns the effect type of the note
  [[nodiscard]] auto getEffect() const -> EffectType;
  // Returns the unique identifier of the note
  [[nodiscard]] auto getId() const -> long unsigned int;
  // Returns the symbol from which this note was created
  [[nodiscard]] auto getSymbol() const -> Symbol;

 private:
  Note(int pitch, int volume, WaveType wave, EffectType effect, string symbol);
  // A number between 0 and 96 describing a note from C0 to B7
  int pitch = 0;
  // A number between 0 and 15 describing the volume of the note
  int volume = 0;
  // The type of wave associated with the note
  WaveType wave = TRIANGLE;
  // The type of effect associated with the note
  EffectType effect = NONE;
  // The unique identifier of the note, used to compare notes
  long int id = 0;
  // The symbol associated with the note
  Symbol symbol = END_OF_TRACK_SYMBOL;
};

using Track = std::vector<Note>;

}  // namespace sid
