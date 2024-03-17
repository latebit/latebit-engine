#pragma once

#include <array>
#include <bitset>

using namespace std;

namespace lb {
// A bitset representing a single 8x8 glyph
using Glyph = bitset<64>;

// A list of 96 glyphs, representing the printable ASCII characters
using GlyphList = array<Glyph, 96>;

// A Font encompasses a list of glyphs and some metrics to print them
class Font {
 private:
  // The list of glyphs composing the font, they represent all the printable
  // ASCII characters (from 32 to 126, inclusive)
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