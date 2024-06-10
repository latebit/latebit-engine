#include "Note.h"

#include <sstream>
#include <vector>

#include "latebit/utils/Logger.h"

const char NULL_CHAR = '-';
const int SYMBOL_SIZE = 6;

auto hexToInt(char hexChar) -> int {
  if (hexChar >= '0' && hexChar <= '9') {
    return hexChar - '0';
  } else if (hexChar >= 'a' && hexChar <= 'f') {
    return hexChar - 'a' + 10;
  } else if (hexChar >= 'A' && hexChar <= 'F') {
    return hexChar - 'A' + 10;
  }
  return 0;
}

auto digitToInt(char c) -> int { return c - '0'; }

namespace sid {

// Highlight the position of the error in the symbol with parentheses
// This assumes the symbol has size SYMBOL_SIZE
auto highlightErrorPosition(const Symbol& symbol,
                            int position) -> const string {
  string result = "";
  for (int i = 0; i < SYMBOL_SIZE; i++) {
    if (i == position) {
      result += "[";
    }
    result += symbol[i];
    if (i == position) {
      result += "]";
    }
  }
  return result;
}

template <int T>
auto validate(const Symbol& symbol, int position, const array<char, T>& allowed,
              const char* message) -> int {
  const char c = symbol[position];

  if (c == NULL_CHAR) return 0;

  for (int i = 0; i < allowed.size(); i++) {
    if (c == allowed[i]) return 0;
  }

  ostringstream s;
  s << message << " in symbol \"" << symbol << "\". Got " << c
    << ", expected one of " << allowed[0] << "-" << allowed[allowed.size() - 1]
    << " or -. Invalid symbol in brackets: "
    << highlightErrorPosition(symbol, position);

  Log.error(s.str().c_str());
  return 1;
}

Note::Note(int pitch, int volume, WaveType wave, EffectType effect,
           string symbol)
  : pitch(pitch),
    volume(volume),
    wave(wave),
    effect(effect),
    symbol(std::move(symbol)) {
  static int id = 0;
  this->id = id++;
}

auto Note::makeRest() -> Note {
  auto note = Note(0, 0, TRIANGLE, NONE, "------");
  note.id = -1;
  return note;
}

auto Note::makeInvalid() -> Note {
  auto note = Note(0, 0, TRIANGLE, NONE, "      ");
  note.id = -2;
  return note;
}

auto Note::makeContinue() -> Note {
  auto note = Note(0, 0, TRIANGLE, NONE, "......");
  note.id = -3;
  return note;
}

const array<char, 7> VALID_NOTES = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
const array<char, 2> VALID_ACCIDENTALS = {'#', 'b'};
const array<char, 8> VALID_OCTAVE = {'0', '1', '2', '3', '4', '5', '6', '7'};
const array<char, 4> VALID_WAVE = {'0', '1', '2', '3'};
const array<char, 16> VALID_VOLUME = {'0', '1', '2', '3', '4', '5', '6', '7',
                                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const array<char, 5> VALID_EFFECT = {'0', '1', '2', '3', '4'};

auto Note::fromSymbol(const Symbol& str) -> Note {
  if (str.size() != SYMBOL_SIZE) {
    Log.error("Invalid string size. Got %d, expected %d\n", (int)str.size(),
              SYMBOL_SIZE);
    return Note::makeInvalid();
  }

  int invalid = 0;
  invalid += validate<7>(str, 0, VALID_NOTES, "Invalid note");
  invalid += validate<2>(str, 1, VALID_ACCIDENTALS, "Invalid accidental");
  invalid += validate<8>(str, 2, VALID_OCTAVE, "Invalid octave");
  invalid += validate<4>(str, 3, VALID_WAVE, "Invalid wave");
  invalid += validate<16>(str, 4, VALID_VOLUME, "Invalid volume");
  invalid += validate<5>(str, 5, VALID_EFFECT, "Invalid effect");

  if (invalid) {
    return Note::makeInvalid();
  }

  int pitch = 0;
  int volume = 0;
  EffectType effect = NONE;
  WaveType wave = TRIANGLE;

  if (str[0] == 'C') {
    pitch = 0;
  } else if (str[0] == 'D') {
    pitch = 2;
  } else if (str[0] == 'E') {
    pitch = 4;
  } else if (str[0] == 'F') {
    pitch = 5;
  } else if (str[0] == 'G') {
    pitch = 7;
  } else if (str[0] == 'A') {
    pitch = 9;
  } else if (str[0] == 'B') {
    pitch = 11;
  } else if (str[0] == NULL_CHAR) {
    return Note::makeRest();
  }

  if (str[1] == '#') {
    pitch++;
  } else if (str[1] == 'b') {
    pitch--;
  }

  if (str[3] == '0' || str[3] == NULL_CHAR) {
    wave = TRIANGLE;
  } else if (str[3] == '1') {
    wave = SAWTOOTH;
  } else if (str[3] == '2') {
    wave = SQUARE;
  } else if (str[3] == '3') {
    wave = NOISE;
  }

  if (str[5] == '0' || str[5] == NULL_CHAR) {
    effect = NONE;
  } else if (str[5] == '1') {
    effect = DROP;
  } else if (str[5] == '2') {
    effect = SLIDE;
  } else if (str[5] == '3') {
    effect = FADEIN;
  } else if (str[5] == '4') {
    effect = FADEOUT;
  }

  int octave = str[2] == NULL_CHAR ? 4 : digitToInt(str[2]);

  /**
  The octave number is tied to the alphabetic character used to describe the
  pitch, with the division between note letters ‘B’ and ‘C’, thus:

  * "B3" and "B#3" are designated as being in octave "3"
  * "C4" and "Cb4" would properly be designated in octave "4"

  even though the pitch is the same.
  */
  if (pitch < 0) {
    pitch = 12 + pitch;
    octave--;
  } else if (pitch > 11) {
    pitch = pitch - 12;
    octave++;
  }
  pitch += (12 * octave);

  volume = str[4] == NULL_CHAR ? 8 : hexToInt(str[4]);

  return {pitch, volume, wave, effect, str};
}

auto Note::isRest() const -> bool { return this->id == -1; }
auto Note::isInvalid() const -> bool { return this->id == -2; }
auto Note::isContinue() const -> bool { return this->id == -3; }

auto Note::isEqual(Note other) const -> bool {
  return this->pitch == other.pitch && this->volume == other.volume &&
         this->wave == other.wave && this->effect == other.effect;
}
auto Note::getPitch() const -> int { return this->pitch; }
auto Note::getVolume() const -> float { return this->volume / 15.0; }
auto Note::getWave() const -> WaveType { return this->wave; }
auto Note::getEffect() const -> EffectType { return this->effect; }
auto Note::getId() const -> long unsigned int { return this->id; }
auto Note::getSymbol() const -> string { return this->symbol; }

}  // namespace sid