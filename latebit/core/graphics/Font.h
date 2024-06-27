#pragma once

#include <array>
#include <bitset>

using namespace std;

namespace lb {
// A bitset representing a single 8x8 glyph
using Glyph = bitset<64>;

// The list of glyphs composing the font.
// Font glyph composition:
// 1. Special Input Symbols (first 10 characters):
//    Order: A, B, L, R, Start, Options, Up, Down, Left, Right
//    These are mapped to rarely used control characters.
//    Note: Do not rely on specific character codes as they may change.
//    See InputGlyph for more information.
//
// 2. Standard ASCII Characters:
//    Includes all printable ASCII (codes 32 to 126 inclusive)
//
// Total glyph count: 10 special + 96 ASCII = 106 glyphs
using GlyphList = array<Glyph, 106>;

namespace InputGlyph {
// Symbols representing input buttons. They can be used to consistently
// display inputs in the game engine. They are meant to be used in strings:
//
//   stringstream msg;
//   msg << "Press " << InputGlyph::START << " to continue."
//   DM.drawString(Vector(), msg.str(), TextAlignment::LEFT, Color::RED);
//
// For this reason they are mapped to ASCII control characters. The actual
// characters used to map the glyphs are _unstable_. Only rely on the enum and
// not on the underlying code!
enum InputGlyph { A = 14, B, L, R, START, OPTIONS, UP, DOWN, LEFT, RIGHT };
}  // namespace InputGlyph

// A Font encompasses a list of glyphs and some metrics to print them
class Font {
 private:
  // The list of glyphs composing the font.
  // Check the GlyphList type for more details on the layout.
  GlyphList glyphs = {};

  // The width of a single glyph in cells
  int glyphWidth = 8;

  // The height of a single glyph in cells
  int glyphHeight = 8;

  // How many cells to leave between characters
  int horizontalSpacing = 1;

 public:
  Font(GlyphList glyphs, int glyphWidth, int glyphHeight,
       int horizontalSpacing);

  // Returns the list of glyphs composing the font
  [[nodiscard]] auto getGlyphs() const -> const GlyphList&;

  // Returns the width of a single glyph in cells
  [[nodiscard]] auto getGlyphWidth() const -> int;

  // Returns the height of a single glyph in cells
  [[nodiscard]] auto getGlyphHeight() const -> int;

  // Returns how many cells to leave between characters
  [[nodiscard]] auto getHorizontalSpacing() const -> int;

  // Returns the glyph corresponding to the given ASCII char
  [[nodiscard]] auto getGlyph(char c) const -> const Glyph&;
};

extern const Font DEFAULT_FONT;
}  // namespace lb