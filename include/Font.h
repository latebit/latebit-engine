#pragma once

#include <array>
#include <bitset>

namespace lb {
using Glyph = std::bitset<64>;
using GlyphList = std::array<Glyph, 96>;

class Font {
 public:
  GlyphList glyphs = {};
  int glyphWidth = 8;
  int glyphHeight = 8;
  int horizontalSpacing = 1;
};

extern const Font DEFAULT_FONT;
}  // namespace lb