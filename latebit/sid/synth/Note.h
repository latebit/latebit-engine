#pragma once

#include <string>
#include <vector>

#include "Oscillator.h"

using namespace std;

namespace sid {
// Symbol is a string that represents a Note. It can be a rest, a continue,
// the end of a track or a note. It is used to parse and generate notes.
using Symbol = string;

const string REST_SYMBOL = "------";
const string CONTINUE_SYMBOL = "......";
const string END_OF_TRACK_SYMBOL = "      ";

namespace NoteType {
enum NoteType {
  STANDARD,
  REST,
  INVALID,
  CONTINUE,
  END_OF_TRACK,
};
};

// A Note is a musical object that represents a sound. It has a pitch, a volume,
// a wave type and an effect type. It also has a unique identifier that can be
// used to compare notes. Notes are immutable, which means that once they are
// created, they cannot be modified.
// A note can also be a rest, which is a symbol that represents silence.
// A note can also be invalid, which is a symbol that represents an invalid note
// and it is used to report errors.
class Note {
 public:
  // Creates a new rest note. This note is used to signal that the note
  // should be silent.
  static auto makeRest() -> Note;
  // Creates a new invalid note. This state exists to report errors
  // instead of doing exception handling. It is used to avoid throwing
  // exceptions when parsing a tune.
  static auto makeInvalid() -> Note;
  // Creates a new continue note. This note is used to signal that the
  // note should continue playing the previous note.
  static auto makeContinue() -> Note;
  // Creates a new note from a symbol
  static auto fromSymbol(const Symbol& str) -> Note;

  // Returns the pitch of the note
  [[nodiscard]] auto getPitch() const -> int;
  // Returns the volume of the note in the range [0, 1]
  [[nodiscard]] auto getVolume() const -> float;
  // Returns the wave type of the note
  [[nodiscard]] auto getWave() const -> WaveType::WaveType;
  // Returns the effect type of the note
  [[nodiscard]] auto getEffect() const -> EffectType::EffectType;
  // Returns the symbol from which this note was created
  [[nodiscard]] auto getSymbol() const -> Symbol;
  // Returns the symbol from which this note was created
  [[nodiscard]] auto getType() const -> NoteType::NoteType;

  // Checks if the given notes are equal (i.e., same pitch, volume, wave)
  auto operator==(const Note& other) const -> bool;
  // Checks if the given notes are not equal
  auto operator!=(const Note& other) const -> bool;

 private:
  Note(int pitch, int volume, WaveType::WaveType wave,
       EffectType::EffectType effect, string symbol);
  // A number between 0 and 96 describing a note from C0 to B7
  int pitch = 0;
  // A number between 0 and 15 describing the volume of the note
  int volume = 0;
  // The type of wave associated with the note
  WaveType::WaveType wave = WaveType::TRIANGLE;
  // The type of effect associated with the note
  EffectType::EffectType effect = EffectType::NONE;
  // The symbol associated with the note
  Symbol symbol = END_OF_TRACK_SYMBOL;
  // The type of the note
  NoteType::NoteType type = NoteType::END_OF_TRACK;
};

using Track = std::vector<Note>;

}  // namespace sid
